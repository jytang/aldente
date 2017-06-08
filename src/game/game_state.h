#pragma once

#include "game_objects/player.h"
#include "physics.h"
#include "scene_manager.h"
#include "scene/main_scene.h"
#include "scene/start_scene.h"
#include "scene/minigame_scenes/mgscene_penguin.h"
#include "scene/minigame_scenes/mgscene_sumo.h"
#include "context.h"
#include "phase/phase.h"
#include "phase/menu.h"
#include "phase/build.h"
#include "phase/dungeon.h"
#include "phase/minigame_phase.h"
#include <map>
#include <unordered_set>
#include <game/phase/dialogue.h>

class GameState {
public:
    GameState() {}

    static Context context;
    static MenuPhase menu_phase;
    static BuildPhase build_phase;
    static DungeonPhase dungeon_phase;
    static MinigamePhase minigame_phase;

    static DialoguePhase build_tutorial_phase;

    static Phase* curr_phase;
    static SceneManager scene_manager;
    static MainScene main_scene;
    static StartScene start_scene;
    static MGScenePenguin penguin_scene;
    static MGSceneSumo sumo_scene;
    static Physics physics;

    static std::map<int, Player*> players;

    // Map of avatar index to bool - true is occupied, false is free.
    static std::map<int, bool> avatar_assignments;

    // Initializes the game state.
    static void setup(bool is_server);

    // Update state given input. Called in the game loop.
    static void s_update();
    static void c_update();

    // Force a phase change to the specified phase.
    // If server, dispatch the change to clients.
    static void set_phase(proto::Phase phase);

    // Adds a player to the main scene. If server, add new player to auto-gen the id.
    // If client, add existing player with given id that the server sent.
    static Player* s_add_player(int conn_id);
    static Player* c_add_player(int obj_id, int model_index, bool is_client);

    // Changes the current scene.
    static void set_scene(Scene* scene);

    // Returns the model index of the next avatar.
    static int cycle_avatar(Player* player);

private:
    static void set_phase(Phase* phase);
    static int num_players;
    static bool is_server;
};