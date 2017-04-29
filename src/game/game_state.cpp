#include "game_state.h"

void GameState::add_phase(PhaseType type, Phase& phase) {
    phases[type] = phase;
}

void GameState::set_phase(PhaseType type) {
    current_phase = type;
}

void GameState::advance_phase() {
    switch (current_phase) {
        case MENU:
            current_phase = BUILD;
            break;
        case BUILD:
            current_phase = DUNGEON;
            break;
        case DUNGEON:
            current_phase = MINIGAME;
            break;
        case MINIGAME:
            current_phase = BUILD;
            break;
        default:
            current_phase = MENU;
            break;
    }
}

PhaseType GameState::get_phase_type() {
    return current_phase;
}

Phase GameState::get_phase() {
    return phases[current_phase];
}

PhaseType GameState::current_phase = PhaseType::MENU;
std::unordered_map<PhaseType, Phase> GameState::phases;
