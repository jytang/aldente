#pragma once

#include "phase.h"

class GameState {
public:
    static MenuPhase menu_phase;
    static BuildPhase build_phase;
    static DungeonPhase dungeon_phase;
    static MinigamePhase minigame_phase;

    static Phase* curr_phase;

    // Initializes the game state with the given start phase.
    static void init(Phase* phase);

    // Update state given input. Called in the game loop.
    // NOTE: not yet implemented; need input to pass in
    static void update();

    // Force a phase change to the specified phase.
    // NOTE: do not use; this is only used for debugging.
    static void set_phase(Phase* phase);
private:

};