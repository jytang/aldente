#pragma once

#include "scene/scene.h"
#include "asset_loader.h"
#include "game_objects/grid.h"
#include "game_objects/player.h"

/*
Main Scene for having stuff in our game, especially for testing.
Anything that you would want to add to the game scene that is only
used for the specific scene should be put here.
In the future, the "Dungeon/Build Phase" would be its own scene, and
then each mini game would likely all be its own scene. The title menu
can also be its own scene.
*/
class MainScene : public Scene {
private:
    Grid *grid;
    Tile *hover;
    bool lights_debug_on = false;
    std::mutex goal_mutex; // In case a new goal is created before old one is removed.
    Goal *goal;
    int goal_z, goal_x;
public:
    MainScene();
    void update() override;
    void client_update() override;
    void setup_scene() override;
    void graphical_setup() override;
    Player* spawn_new_player(); // Server: spawn new Player, auto-assign id.
    Player* spawn_existing_player(int obj_id); // Client: spawn existing Player with id from server.

    // Places goal with minimum distance from the start. 
    // Distance calculated using manhattan distance(x diff + z diff)
    // Note: try not to use a high min dist
    void place_goal(glm::vec3 start, int min_dist);

    void place_existing_goal(int x, int z, int id);

    void remove_goal();
};