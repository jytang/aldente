#pragma once

#include "game_objects/player.h"
#include "physics.h"
#include "scene_manager.h"
#include "context.h"
#include "phase/phase.h"
#include "phase/build.h"
#include "phase/dungeon.h"
#include <map>
#include <unordered_set>

class GameState {
public:
    static Context context;
    static BuildPhase build_phase;
    static DungeonPhase dungeon_phase;

    static Phase* curr_phase;
    static SceneManager scene_manager;
    static Physics physics;

    static std::map<int, Player*> players;

    // Initializes the game state.
    static void setup(bool is_server);

    // Update state given input. Called in the game loop.
    static void s_update();
    static void c_update();

    // Force a phase change to the specified phase.
    // If server, dispatch the change to clients.
    static void set_phase(proto::Phase phase);
    static void set_client_phase(proto::Phase phase);

    // Adds a player to the main scene. If server, add new player to auto-gen the id.
    // If client, add existing player with given id that the server sent.
    static Player* s_add_player(int conn_id);
    static Player* c_add_player(int obj_id, std::string& model_name, bool is_client);
private:
    static void set_phase(Phase* phase);
    static MainScene testScene;
    static int num_players;
    static bool is_server;
};