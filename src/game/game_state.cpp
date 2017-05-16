#include "game_state.h"

Context GameState::context;
BuildPhase GameState::build_phase(context);
DungeonPhase GameState::dungeon_phase(context);

Phase* GameState::curr_phase;
std::map<int, Player*> GameState::players;

Physics GameState::physics;
SceneManager GameState::scene_manager;
MainScene GameState::testScene;
int GameState::num_players = 0;
bool GameState::is_server = true;

void GameState::setup(bool is_server) {
    GameState::is_server = is_server;

    // TODO: shouldn't need physics on client, but this is needed to create tiles & their rigid bodies.
    physics.set_scene(&testScene);
    scene_manager.set_current_scene(&testScene);

    if (is_server) {
        // Client of given connection id wishes to join the game.
        events::menu::request_join_event.connect([](int conn_id) {
            proto::JoinResponse resp;
            resp.set_status(num_players < 4);
            resp.set_id(conn_id);

            // If the game isn't full (< 4 players), create a Player object for the client.
            if (num_players < 4) {
                Player* player = add_new_player();
                resp.set_obj_id(player->get_id());
                players[conn_id] = player;
                num_players++;
            }
            events::menu::respond_join_event(conn_id, resp);
        });
    }
    else {
        scene_manager.get_current_scene()->graphical_setup();

        events::menu::spawn_existing_player_event.connect([](int id) {
            add_existing_player(id, false);
        });
    }
}

void GameState::update() {
    assert(curr_phase);
    Phase* next_phase = curr_phase->update();
    if (next_phase) {
        set_phase(next_phase);
    }
    physics.update();
    scene_manager.get_current_scene()->update();
}

void GameState::client_update() {
    scene_manager.get_current_scene()->client_update();
    curr_phase->client_update();
}

void GameState::set_phase(Phase* phase) {
    // If no change to phase, don't teardown and setup.
    if (phase == curr_phase) return;

    if (is_server) {
        if (curr_phase)
            curr_phase->teardown();
        curr_phase = phase;
        curr_phase->setup();
    }
    else {
        if (curr_phase)
            curr_phase->client_teardown();
        curr_phase = phase;
        curr_phase->client_setup();
    }
}

void GameState::set_phase(proto::Phase phase) {
    switch (phase) {
    case proto::Phase::MENU:
        // FIXME(metakirby5)
        break;
    case proto::Phase::BUILD:
        GameState::set_phase(&GameState::build_phase);
        break;
    case proto::Phase::DUNGEON:
        GameState::set_phase(&GameState::dungeon_phase);
        break;
    case proto::Phase::MINIGAME:
        // FIXME(metakirby5)
        break;
    }
}

Player* GameState::add_new_player() {
    // For now, only create players on the main scene.
    assert(scene_manager.get_current_scene() == &testScene);
    return testScene.spawn_new_player();
}

Player* GameState::add_existing_player(int obj_id, bool is_client) {    
    // For now, only create players on the main scene.
    assert(scene_manager.get_current_scene() == &testScene);

    if (is_client)
        context.player_id = obj_id;
    return testScene.spawn_existing_player(obj_id);
}
