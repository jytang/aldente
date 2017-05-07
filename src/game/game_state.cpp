#include "game_state.h"
#include "events.h"

MenuPhase GameState::menu_phase;
BuildPhase GameState::build_phase;
DungeonPhase GameState::dungeon_phase;
MinigamePhase GameState::minigame_phase;
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
            add_player(conn_id);
    });

    events::menu::spawn_player_event.connect([](proto::Player & player) {
        add_player(player.id());
    });
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

    events::dungeon::network_positions_event(players);    
}

void GameState::set_phase(Phase* phase) {
    curr_phase->teardown();
    curr_phase = phase;
    curr_phase->setup();
}

void GameState::add_player(int conn_id) {
    Player* player = dynamic_cast<MainScene*>(scene_manager.get_current_scene())->spawn_player(conn_id);
    players[conn_id] = player;
    num_players++;
}
