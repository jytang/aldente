#pragma once

#include "game_objects/player.h"
#include "physics.h"
#include "scene_manager.h"
#include "phase/phase.h"
#include "phase/build.h"
#include "phase/dungeon.h"

class GameState {
public:
    static BuildPhase build_phase;
    static DungeonPhase dungeon_phase;

    static Phase* curr_phase;
    static SceneManager scene_manager;
    static Physics physics;

    static std::map<int, Player*> players;

    // Initializes the game state with the given start phase.
    static void init(Phase* phase);

    // Performs graphical setup on the current scene. Only for the client.
    static void graphical_setup();

    // Update state given input. Called in the game loop.
    // NOTE: not yet implemented; need input to pass in
    static void update();
    static void client_update();

    // Force a phase change to the specified phase.
    static void set_phase(Phase* phase);
    static void set_phase(proto::Phase phase);

    // Adds a player to the main scene with given id.
    // Specify if we should set up graphics for the player model (client-side).
    static void add_player(int conn_id, bool graphical);
private:
    static MainScene testScene;
    static int num_players;
};