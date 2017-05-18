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
public:
    MainScene();
    void s_update() override;
    void c_update() override;
    void setup_scene() override;
    void graphical_setup() override;

    Player* s_spawn_player(int conn_id); // Server: spawn new Player, auto-assign id.
    Player* c_spawn_player(int obj_id); // Client: spawn existing Player with id from server.
};