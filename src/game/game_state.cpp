#include "game_state.h"

MenuPhase GameState::menu_phase;
BuildPhase GameState::build_phase;
DungeonPhase GameState::dungeon_phase;
MinigamePhase GameState::minigame_phase;
Phase* GameState::curr_phase;


void GameState::init(Phase* phase) {
    curr_phase = phase;
    curr_phase->setup();
}

void GameState::update() {
    Phase* next_phase = curr_phase->update();
    if (next_phase) {
        curr_phase->teardown();
        curr_phase = next_phase;
        curr_phase->setup();
    }
}

void GameState::set_phase(Phase* phase) {
    curr_phase->teardown();
    curr_phase = phase;
    curr_phase->setup();
}
