#include "game_state.h"

BuildPhase GameState::build_phase;
DungeonPhase GameState::dungeon_phase;

Phase* GameState::curr_phase;
std::map<int, Player*> GameState::players;

Physics GameState::physics;
SceneManager GameState::scene_manager;
MainScene GameState::testScene;
int GameState::num_players = 0;

void GameState::init(Phase* phase) {
    curr_phase = phase;
    curr_phase->setup();

    physics.set_scene(&testScene);
    scene_manager.set_current_scene(&testScene);

    events::menu::request_join_event.connect([](int conn_id) {
        proto::JoinResponse resp;
        resp.set_status(num_players < 4);
        resp.set_id(conn_id);
        events::menu::respond_join_event(conn_id, resp);

        if (num_players < 4)
            add_player(conn_id, false);
    });

    events::menu::spawn_player_event.connect([](proto::Player & p) {
        add_player(p.id(), true);
    });
}

void GameState::graphical_setup() {
    scene_manager.get_current_scene()->graphical_setup();
}

void GameState::update() {
    Phase* next_phase = curr_phase->update();
    if (next_phase) {
        curr_phase->teardown();
        curr_phase = next_phase;
        curr_phase->setup();
    }
    physics.update();
    scene_manager.get_current_scene()->update();

    if (curr_phase == &dungeon_phase) {
        events::dungeon::network_positions_event(players);
    }
}

void GameState::client_update() {
    scene_manager.get_current_scene()->client_update();
}

void GameState::set_phase(Phase* phase) {
    curr_phase->teardown();
    curr_phase = phase;
    curr_phase->setup();
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


void GameState::add_player(int conn_id, bool graphical) {
    assert(players.find(conn_id) == players.end());
    
    // For now, only create players on the main scene.
    assert(scene_manager.get_current_scene() == &testScene);
    Player* player = testScene.spawn_player(conn_id, graphical);
    players[conn_id] = player;
    num_players++;
}
