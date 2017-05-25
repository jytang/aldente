#include "network_manager.h"
#include "util/config.h"
#include "game_objects/player.h"
#include "game_objects/essence.h"
#include "game_objects/traps/slime.h"
#include "game/game_state.h"
#include <GLFW/glfw3.h>
#include <unordered_set>

void NetworkManager::disconnect() {
    if (!io_service.stopped())
        io_service.stop();
    if (service_thread) {
        service_thread->join();
        delete service_thread;
    }
}

void NetworkManager::run_service() {
    while (service_thread && !io_service.stopped()) {
        try {
            io_service.poll();
        } catch (std::exception & ex) {
            // Silently ignore errors rather than crash. Shouldn't happen.
            std::cerr << "run_service error: " << ex.what() << "\n";
        }
    }
}

void ServerNetworkManager::connect() {
    register_listeners();
    service_thread = new boost::thread(&NetworkManager::run_service, this);
}

void ServerNetworkManager::register_listeners() {
    // Generic message to all.
    events::server::announce.connect([&](proto::ServerMessage &message) {
        server.send_to_all(message);
    });

    // Menu phase.
    events::menu::respond_join_event.connect([&](int conn_id, proto::JoinResponse& resp) {
        proto::ServerMessage msg;
        msg.set_allocated_join_response(new proto::JoinResponse(resp));
        server.send_to(conn_id, msg);

        // Then, send the state of all players to all clients.
        proto::ServerMessage update_msg;
        proto::GameState* state = new proto::GameState();
        for (auto & kv : GameState::players) {
            proto::GameObject* go = state->add_objects();
            Player* player = kv.second;
            go->set_id(player->get_id());
            go->set_type(proto::GameObject::Type::GameObject_Type_PLAYER);
            go->set_model_index(player->c_get_model_index());
            go->set_x(player->transform.get_position().x);
            go->set_z(player->transform.get_position().z);
            go->set_wx(player->transform.get_forward().x);
            go->set_wz(player->transform.get_forward().z);
            go->set_enabled(player->is_enabled());
        }
        
        update_msg.set_allocated_state_update(state);
        server.send_to_all(update_msg);
    });

    // Build phase.
    events::build::respond_build_event.connect([&](proto::Construct& c) {
        proto::ServerMessage msg;
        msg.set_allocated_build_update(new proto::Construct(c));
        server.send_to_all(msg);
    });

    // Dungeon phase.
    events::dungeon::update_state_event.connect([&](Context* context) {
        proto::ServerMessage msg;
        proto::GameState* state = new proto::GameState();

        // Updating the client positions & orientations based on sent server positions.
        for (auto & obj : context->updated_objects) {
            proto::GameObject* go = state->add_objects();
            go->set_id(obj->get_id());
            Player* player;

            if ((player = dynamic_cast<Player*>(obj))) {
                go->set_type(proto::GameObject::Type::GameObject_Type_PLAYER);
                go->set_model_index(player->c_get_model_index());
            }
            else if (dynamic_cast<Goal*>(obj))
                go->set_type(proto::GameObject::Type::GameObject_Type_GOAL);
            else if (dynamic_cast<Essence*>(obj))
                go->set_type(proto::GameObject::Type::GameObject_Type_ESSENCE);
            go->set_x(obj->transform.get_position().x);
            go->set_z(obj->transform.get_position().z);
            go->set_wx(obj->transform.get_forward().x);
            go->set_wz(obj->transform.get_forward().z);
            go->set_enabled(obj->is_enabled());
        }

        // If there were any game obj collisions, send those objects' ids.
        for (const auto &collision : context->collisions) {
            auto *cur = state->add_collisions();
            cur->set_initiator(collision.first);
            cur->set_other(collision.second);
        }

        // If there were any game obj interacts, send those objects' ids.
        for (const auto &interact : context->interacts) {
            auto *cur = state->add_interacts();
            cur->set_initiator(interact.first);
            cur->set_other(interact.second);
        }

        context->updated_objects.clear();
        context->collisions.clear();
        context->interacts.clear();

        msg.set_allocated_state_update(state);
        server.send_to_all(msg);
    });
}

void ServerNetworkManager::update() {
    for (auto& client : server.read_all_messages()) {
        for (auto& msg : client.second) {
            switch (msg.message_type_case()) {
            case proto::ClientMessage::MessageTypeCase::kBuildRequest: {
                proto::Construct construct = msg.build_request();
                events::build::s_verify_and_build(construct);
                break;
            }
            case proto::ClientMessage::MessageTypeCase::kMoveRequest: {
                proto::StickData stick = msg.move_request();
                if (stick.input() == proto::StickData::STICK_LEFT)
                    GameState::players[stick.id()]->prepare_movement(stick.x(), stick.y());
                break;
            }
            case proto::ClientMessage::MessageTypeCase::kPhaseRequest: {
                proto::Phase phase = msg.phase_request();
                GameState::set_phase(phase);

                // Announce phase change to all clients.
                proto::ServerMessage phase_announce;
                phase_announce.set_phase_update(phase);
                events::server::announce(phase_announce);
                break;
            }
            case proto::ClientMessage::MessageTypeCase::kInteractRequest: {
                // Server received the client ID of the player that wants to interact.
                // Perform raycasting and check if any game object actually got interacted.
                int id = GameState::players[msg.interact_request()]->get_id();
                Player* player = dynamic_cast<Player*>(GameObject::game_objects[id]);
                player->interact();
                break;
            }
            case proto::ClientMessage::MessageTypeCase::kReadyRequest: {
                events::build::player_ready_event(msg.ready_request());
                break;
            }
            case proto::ClientMessage::MessageTypeCase::kChangeAvatarRequest: {
                proto::AvatarChange change = msg.change_avatar_request();
                Player* player = dynamic_cast<Player*>(GameObject::game_objects[change.player_id()]);
                assert(player);
                player->s_set_model_index(change.model_index());

                // Send the avatar change update to all clients.
                proto::ServerMessage response;
                response.set_allocated_change_avatar_update(new proto::AvatarChange(change));
                server.send_to_all(response);
                break;
            }
            default:
                break;
            }
        }
    }
}

void ClientNetworkManager::connect() {
    Config::config->get_value(Config::str_server_ip, server_host);
    Config::config->get_value(Config::str_port, port);

    // Try establishing connection in separate thread.
    boost::thread(&ClientNetworkManager::attempt_connection, this).detach();
}

void ClientNetworkManager::register_listeners() {
    // Generic message to server.
    events::client::send.connect([&](proto::ClientMessage &message) {
        client.send(message);
    });

    // Debug.
    events::debug::client_set_phase_event.connect([&](Phase* phase) {
        proto::ClientMessage msg;
        /* if (phase == &GameState::menu_phase)               // FIXME(metakirby5)
            msg.set_phase_request(proto::Phase::MENU);
        else */ if (phase == &GameState::build_phase)
            msg.set_phase_request(proto::Phase::BUILD);
        else if (phase == &GameState::dungeon_phase)
            msg.set_phase_request(proto::Phase::DUNGEON);
//        else if (phase == &GameState::minigame_phase)
//            msg.set_phase_request(proto::Phase::MINIGAME);  // FIXME(metakirby5)
        else {
            std::cerr << "Unrecognized phase. Use the static phases in GameState.\n";
            return;
        }
        client.send(msg);
    });

    // Build phase.
    events::build::request_build_event.connect([&](proto::Construct& c) {
        proto::ClientMessage msg;
        c.set_player_id(client_id);
        msg.set_allocated_build_request(new proto::Construct(c));
        client.send(msg);
    });

    // Dungeon phase.
    events::dungeon::network_player_move_event.connect([&](events::StickData d) {
        proto::ClientMessage msg;
        proto::StickData* pd = new proto::StickData();
        pd->set_input(
            d.input == events::Stick::STICK_LEFT ? 
            proto::StickData::STICK_LEFT :
            proto::StickData::STICK_RIGHT);
        pd->set_x(d.state.first);
        pd->set_y(d.state.second);
        pd->set_id(client_id);
        msg.set_allocated_move_request(pd);
        client.send(msg);
    });

    events::dungeon::player_interact_event.connect([&]() {
        proto::ClientMessage msg;
        msg.set_interact_request(client_id);
        client.send(msg);
    });
}

void ClientNetworkManager::update() {
    proto::ServerMessage msg;
    while (client.read_message(&msg)) {
        switch (msg.message_type_case()) {
        case proto::ServerMessage::MessageTypeCase::kBuildUpdate: {
            proto::Construct construct = msg.build_update();
            if (construct.status())
                events::build::update_build_event(construct);
            else {
                // TODO: construct placement failed. client-side notification?
            }
            break;
        }
        case proto::ServerMessage::MessageTypeCase::kJoinResponse: {
            proto::JoinResponse resp = msg.join_response();
            // If the server successfully added this client to the game, create a local Player object.
            if (resp.status()) {
                client_id = resp.id();
                GameState::c_add_player(resp.obj_id(), resp.model_index(), true)->c_set_client_player();
            }
            break;
        }
        case proto::ServerMessage::MessageTypeCase::kStateUpdate: {
            proto::GameState state = msg.state_update();

            for (auto obj : state.objects()) {
                if (GameObject::game_objects.find(obj.id()) == GameObject::game_objects.end()) {
                    // Game object with that ID doesn't exist on this client yet; create it.
                    if (obj.type() == proto::GameObject::Type::GameObject_Type_PLAYER) {
                        events::menu::spawn_existing_player_event(obj.id(), obj.model_index());
                    } else if (obj.type() == proto::GameObject::Type::GameObject_Type_GOAL) {
                        events::dungeon::spawn_existing_goal_event(obj.x(), obj.z(), obj.id());
                    } else if (obj.type() == proto::GameObject::Type::GameObject_Type_ESSENCE) {
                        events::dungeon::c_spawn_essence_event(obj.x(), obj.z(), obj.id());
                    } else {
                        std::cerr << "Unrecognized game obj type; could not create client copy.\n";
                    }
                } else {
                    GameObject::game_objects[obj.id()]->c_update_state(obj.x(), obj.z(), obj.wx(), obj.wz(), obj.enabled());
                }
            }

            // Call all collision handlers of game objects that collided, but only ones that already exist, 
            // which avoids collision of a not-yet-created game obj.
            for (auto &p : state.collisions()) {
                GameObject* other = GameObject::game_objects[p.other()];
                GameObject* initiator = GameObject::game_objects[p.initiator()];
                if (other && initiator)
                    other->c_on_collision(initiator);
            }
            for (auto &p : state.interacts()) {
                GameObject* other = GameObject::game_objects[p.other()];
                GameObject* initiator = GameObject::game_objects[p.initiator()];
                if (other && initiator)
                    other->c_interact_trigger(initiator);
            }
            break;
        }
        case proto::ServerMessage::MessageTypeCase::kPhaseUpdate: {
            proto::Phase phase = msg.phase_update();
            GameState::set_phase(phase);
            break;
        }
        case proto::ServerMessage::MessageTypeCase::kPlayerFinished: {
            events::player_finished_event(msg.player_finished());
            break;
        }
        case proto::ServerMessage::MessageTypeCase::kPlayerStatsUpdate: {
            const auto &update = msg.player_stats_update();

            // Find the player in question and update their status
            Player *player = dynamic_cast<Player *>(GameObject::game_objects[update.id()]);
            assert(player);
            player->c_update_stats(update);

            break;
        }
        case proto::ServerMessage::MessageTypeCase::kTimeUpdate: {
            // Update the graphical timer
            events::ui::update_time(msg.time_update());
            break;
        }
        case proto::ServerMessage::MessageTypeCase::kChangeAvatarUpdate: {
            proto::AvatarChange change = msg.change_avatar_update();
            Player* player = dynamic_cast<Player*>(GameObject::game_objects[change.player_id()]);
            assert(player);
            player->c_setup_player_model(change.model_index());
            break;
        }
        default:
            break;
        }
    }
}

void ClientNetworkManager::attempt_connection() {
    bool first_attempt = true;
    double time_last_connect_attempt = glfwGetTime();

    // Try connecting to the host every 5 seconds.
    while (!is_connected) {
        if (first_attempt || glfwGetTime() - time_last_connect_attempt > CONN_RETRY_SEC) {
            first_attempt = false;
            std::cerr << "Attempting to connect to " << server_host << "...\n";
            is_connected = client.connect(server_host, port);
            time_last_connect_attempt = glfwGetTime();
            if (is_connected) {
                std::cerr << "Established connection.\n";
                register_listeners();
                service_thread = new boost::thread(&NetworkManager::run_service, this);
            }
        }
    }
}
