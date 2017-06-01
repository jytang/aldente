#include "game_state.h"

Context GameState::context;
MenuPhase GameState::menu_phase(context);
BuildPhase GameState::build_phase(context);
DungeonPhase GameState::dungeon_phase(context);
MinigamePhase GameState::minigame_phase(context);

Phase* GameState::curr_phase;
std::map<int, Player*> GameState::players;

Physics GameState::physics;
SceneManager GameState::scene_manager;
MainScene GameState::main_scene;
StartScene GameState::start_scene;
MGScenePenguin GameState::penguin_scene;
int GameState::num_players = 0;
bool GameState::is_server = true;

void GameState::setup(bool is_server) {
    GameState::is_server = is_server;
    scene_manager.add_scene(&start_scene);
    scene_manager.add_scene(&main_scene);
    scene_manager.add_scene(&penguin_scene);

    if (is_server) {
        // Setup the main scene first, since we want the grid/tiles to be created first.
        physics.set_scene(&main_scene);
        main_scene.s_setup();
        physics.set_scene(&start_scene);
        start_scene.s_setup();
        physics.set_scene(&penguin_scene);
        penguin_scene.s_setup();

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

        events::menu::spawn_existing_player_event.connect([](int id, int model_index) {
            context.player_ids.push_back(id);
            c_add_player(id, model_index, false);
        });
    }

    events::menu::end_menu_event.connect([&]() {
        set_scene(&main_scene);
    });

    events::minigame::start_minigame_event.connect([&]() {
        set_scene(&penguin_scene);
    });

    events::minigame::end_minigame_event.connect([&]() {
        set_scene(&main_scene);
    });

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
        if (curr_phase)
            curr_phase->c_teardown();
        curr_phase = phase;
        curr_phase->c_setup();
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
        events::disable_rigidbody_event(kv.second);
    }

    physics.set_scene(scene);
    scene_manager.set_current_scene(scene);

    // Enable rigid bodies on the next scene.
    for (auto & kv : players) {
        events::enable_rigidbody_event(kv.second);
    }
}

Player* GameState::s_add_player(int conn_id) {
    Player *player = new Player();

    // Determine where each player starts based on client id. 
    player->set_start_position({ (2 * (conn_id - 1)), 0, 0 });
    player->s_set_model_index(conn_id % Player::PLAYER_MODELS.size());
    player->reset_position();

    start_scene.objs.push_back(player);
    main_scene.objs.push_back(player);
    penguin_scene.objs.push_back(player);

    return player;
}

Player* GameState::c_add_player(int obj_id, int model_index, bool is_client) {
    Player *player = new Player(obj_id);
    player->c_setup_player_model(model_index);
    
    start_scene.objs.push_back(player);
    main_scene.objs.push_back(player);
    penguin_scene.objs.push_back(player);

    if (is_client) {
        context.player_id = obj_id;
        context.client_player = player;
    }

    return player;
}
