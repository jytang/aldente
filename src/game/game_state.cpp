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
        // For now, allow more than 4 players to join the game.
        events::menu::request_join_event.connect([](int conn_id) {
            proto::JoinResponse resp;
            resp.set_status(true);
            resp.set_id(conn_id);

            if (players.find(conn_id) != players.end()) {
                // If this is a reconnection, reuse the existing player.
                Player *player = players[conn_id];
                resp.set_obj_id(player->get_id());
            } else {
                // Need to create a new player for the client.
                Player *player = add_new_player(conn_id);
                resp.set_obj_id(player->get_id());
                context.player_ids.push_back(player->get_id());
                players[conn_id] = player;
                num_players++;
            }

            events::menu::respond_join_event(conn_id, resp);
        });
    }
    else {
        scene_manager.get_current_scene()->graphical_setup();

        events::menu::spawn_existing_player_event.connect([](int id) {
            c_add_player(id, false);
        });
    }
}

void GameState::update() {
    assert(curr_phase);
    set_phase(curr_phase->update());
    scene_manager.get_current_scene()->s_update();
}

void GameState::client_update() {
    scene_manager.get_current_scene()->c_update();
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
    case proto::Phase::NOOP:
        return;
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

    if (is_server) {
        // Announce the phase
        proto::ServerMessage phase_announce;
        phase_announce.set_phase_update(phase);
        events::server::announce(phase_announce);
    }
}

Player* GameState::s_add_player(int conn_id) {
    // For now, only create players on the main scene.
    assert(scene_manager.get_current_scene() == &testScene);
    return testScene.s_spawn_player(conn_id);
}

Player* GameState::c_add_player(int obj_id, bool is_client) {    
    // For now, only create players on the main scene.
    assert(scene_manager.get_current_scene() == &testScene);

    if (is_client)
        context.player_id = obj_id;
    return testScene.c_spawn_player(obj_id);
}
