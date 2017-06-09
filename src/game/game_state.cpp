#include "game_state.h"
#include "dialogues.h"

Context GameState::context;
MenuPhase GameState::menu_phase(context);
BuildPhase GameState::build_phase(context);
DungeonPhase GameState::dungeon_phase(context);
MinigamePhase GameState::minigame_phase(context);
MinigameResultsPhase GameState::minigame_results_phase(context);
EndPhase GameState::end_phase(context);

DialoguePhase GameState::build_tutorial_phase(
        context, proto::Phase::BUILD, "Build Tutorial",
        dialogue::BUILD_TUT, &GameState::main_scene,
        [](Camera &cam) {
            cam.cam_pos = {0, 3, 0}; // Top left corner
            cam.rotate_cam({0, 1, 0}, -(90 + 45)); // Face the grid
        });
DialoguePhase GameState::dungeon_tutorial_phase(
        context, proto::Phase::DUNGEON, "Dungeon Tutorial",
        dialogue::DUNGEON_TUT, &GameState::main_scene,
        [](Camera &cam) {
            cam.cam_pos = {0, 3, 0}; // Top left corner
            cam.rotate_cam({0, 1, 0}, -(90 + 45)); // Face the grid
        });
DialoguePhase GameState::minigame_tutorial_phase(
        context, proto::Phase::MINIGAME, "Minigame Tutorial",
        dialogue::MINIGAME_TUT, &GameState::main_scene,
        [](Camera &cam) {
            cam.cam_pos = {0, 3, 0}; // Top left corner
            cam.rotate_cam({0, 1, 0}, -(90 + 45)); // Face the grid
        });

Phase* GameState::curr_phase;
std::map<int, Player*> GameState::players;
std::map<int, bool> GameState::avatar_assignments = {
    {0, false}, {1, false}, {2, false}, {3, false}
};

Physics GameState::physics;
SceneManager GameState::scene_manager;
MainScene GameState::main_scene;
StartScene GameState::start_scene;
MGScenePenguin GameState::penguin_scene;
MGSceneSumo GameState::sumo_scene;
MGScenePump GameState::pump_scene;

int GameState::num_players = 0;
bool GameState::is_server = true;

void GameState::setup(bool is_server) {
    GameState::is_server = is_server;

    // Adds minigame scenes.
    context.minigame_scenes["penguin"] = &penguin_scene;
    context.minigame_scenes["sumo"] = &sumo_scene;
    context.minigame_scenes["pump"] = &pump_scene;

    scene_manager.add_scene(&start_scene);
    scene_manager.add_scene(&main_scene);
    scene_manager.add_scene(&penguin_scene);
    scene_manager.add_scene(&sumo_scene);
    scene_manager.add_scene(&pump_scene);

    if (is_server) {
        // Setup the main scene first, since we want the grid/tiles to be created first.
        physics.set_scene(&main_scene);
        main_scene.s_setup();
        physics.set_scene(&start_scene);
        start_scene.s_setup();
        physics.set_scene(&penguin_scene);
        penguin_scene.s_setup();
        physics.set_scene(&sumo_scene);
        sumo_scene.s_setup();
        physics.set_scene(&pump_scene);
        pump_scene.s_setup();

        // Client of given connection id wishes to join the game.
        // For now, allow more than 4 players to join the game.
        events::menu::request_join_event.connect([](int conn_id) {
            proto::JoinResponse resp;
            resp.set_status(true);
            resp.set_id(conn_id);

            Player* player;
            if (players.find(conn_id) != players.end()) {
                // If this is a reconnection, reuse the existing player.
                player = players[conn_id];
                resp.set_obj_id(player->get_id());
            } else {
                // Need to create a new player for the client.
                player = s_add_player(conn_id);
                resp.set_obj_id(player->get_id());
                context.player_ids.push_back(player->get_id());
                context.ready_flags[player->get_id()] = false;
                players[conn_id] = player;
                num_players++;
            }

            resp.set_model_index(player->c_get_model_index());
            avatar_assignments[player->c_get_model_index()] = true;
            events::menu::respond_join_event(conn_id, resp);
        });
    }
    else {
        // Setup the main scene first, since we want the grid/tiles to be created first.
        physics.set_scene(&main_scene);
        main_scene.c_setup();
        physics.set_scene(&start_scene);
        start_scene.c_setup();
        physics.set_scene(&penguin_scene);
        penguin_scene.c_setup();
        physics.set_scene(&sumo_scene);
        sumo_scene.c_setup();
        physics.set_scene(&pump_scene);
        pump_scene.c_setup();

        events::menu::spawn_existing_player_event.connect([&](int id, int model_index) {
            c_add_player(id, model_index, false);
        });
    }

    physics.set_scene(&start_scene);
    scene_manager.set_current_scene(&start_scene);
}

void GameState::s_update() {
    assert(curr_phase);
    set_phase(curr_phase->s_update());
    scene_manager.get_current_scene()->s_update();
}

void GameState::c_update() {
    scene_manager.get_current_scene()->c_update();
    curr_phase->c_update();
}

void GameState::set_phase(Phase* phase) {
    // If no change to phase, don't teardown and setup.
    if (phase == curr_phase) return;

    if (is_server) {
        if (curr_phase)
            curr_phase->s_teardown();
        curr_phase = phase;
        curr_phase->s_setup();
    }
    else {
        // Don't teardown/transition if it is the first phase.
        if (curr_phase == nullptr) {
            curr_phase = phase;
            curr_phase->c_setup();
        } else {
            // Hacky: don't do transition for minigame results phase.
            if (phase == &minigame_results_phase) {
                curr_phase->c_teardown();
                curr_phase = phase;
                curr_phase->c_setup();
            } else {
                // Transition, then change phase as a callback at the "apex" of the transition.
                events::ui::transition_wipe(1.5f, phase->to_string(), [&, phase]() {
                    curr_phase->c_teardown();
                    curr_phase = phase;
                    curr_phase->c_setup();
                });
            }
        }
    }
}

void GameState::set_phase(proto::Phase phase) {

    switch (phase) {
    case proto::Phase::NOOP:
        return;
    case proto::Phase::MENU:
        GameState::set_phase(&GameState::menu_phase);
        break;
    case proto::Phase::BUILD:
        GameState::set_phase(&GameState::build_phase);
        break;
    case proto::Phase::DUNGEON:
        GameState::set_phase(&GameState::dungeon_phase);
        break;
    case proto::Phase::MINIGAME:
        GameState::set_phase(&GameState::minigame_phase);
        break;
    case proto::Phase::MINIGAME_RESULTS:
        GameState::set_phase(&GameState::minigame_results_phase);
        break;
    case proto::Phase::BUILD_TUTORIAL:
        GameState::set_phase(&GameState::build_tutorial_phase);
        break;
    case proto::Phase::DUNGEON_TUTORIAL:
        GameState::set_phase(&GameState::dungeon_tutorial_phase);
        break;
    case proto::Phase::MINIGAME_TUTORIAL:
        GameState::set_phase(&GameState::minigame_tutorial_phase);
        break;
    case proto::Phase::END:
        GameState::set_phase(&GameState::end_phase);
        break;
    }

    if (is_server) {
        // Announce the phase
        proto::ServerMessage phase_announce;
        phase_announce.set_phase_update(phase);
        events::server::announce(phase_announce);
    }
}

void GameState::set_scene(Scene* scene) {
    // Disable rigid bodies on the previous scene.
    for (auto & kv : players) {
        kv.second->disable();
    }

    physics.set_scene(scene);
    scene->reset_camera();
    scene->reset_scene();
    scene_manager.set_current_scene(scene);

    // Enable rigid bodies on the next scene.
    for (auto & kv : players) {
        kv.second->enable();
    }
}

Player* GameState::s_add_player(int conn_id) {
    Player *player = new Player();

    // Determine where each player starts based on client id. 
    player->set_start_position({ (2 * (conn_id - 1)), 0, 0 });
    player->s_set_model_index(-1);
    player->s_set_model_index(cycle_avatar(player));
    player->reset_position();

    start_scene.objs.push_back(player);
    main_scene.objs.push_back(player);
    penguin_scene.objs.push_back(player);
    sumo_scene.objs.push_back(player);
    pump_scene.objs.push_back(player);

    return player;
}

Player* GameState::c_add_player(int obj_id, int model_index, bool is_client, int pos) {
    Player *player = new Player(obj_id);
    player->c_setup_player_model(model_index);
    context.player_ids.push_back(obj_id);

    start_scene.objs.push_back(player);
    main_scene.objs.push_back(player);
    penguin_scene.objs.push_back(player);
    sumo_scene.objs.push_back(player);
    pump_scene.objs.push_back(player);

    if (is_client) {
        context.player_id = obj_id;
        context.client_player = player;

        // Add a light to illuminate the player's bed.
        float x_pos = 2 * (pos - 1);
        auto goal_light = new PulsePointLight(glm::vec3(x_pos, 0.5f, 0), Color::PURPLE, 0.1f, 1.f, 0.02f, 0.1f);
        start_scene.add_light(goal_light);
    }
    sumo_scene.c_add_ball(player);

    return player;
}

int GameState::cycle_avatar(Player* player) {
    int old = player->c_get_model_index();

    if (old == -1) {
        for (int i = 0; i < 4; i++) {
            if (!avatar_assignments[i]) {
                avatar_assignments[i] = true;
                return i;
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        int current = (old + i) % 4;
        // This avatar is currently not in use. Assign to player.
        if (!avatar_assignments[current]) {
            avatar_assignments[old] = false;
            avatar_assignments[current] = true;
            return current;
        }
    }

    // No change in avatar, since all are taken.
    return old;
}

