#pragma once

#include "scene/scene.h"

/*
Scene for penguin minigame
*/

const auto MAX_PENGUINS = 40;

class MGScenePenguin : public Scene {
private:
public:
    MGScenePenguin() {};
    void s_update() override;
    void c_update() override;
    void s_setup() override;
    void c_setup() override;
    void connect_listeners() override;
    void disconnect_listeners() override;
};