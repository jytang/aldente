#pragma once

#include "scene/scene.h"
#include "asset_loader.h"
#include "game_objects/grid.h"

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
    void update() override;
    void setup_scene() override;
};