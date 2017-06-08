#pragma once

#include "scene/scene.h"
#include "game_objects/minigame/penguin.h"

/*
Scene for penguin minigame
*/

const auto MAX_PENGUINS = 80;

class MGScenePenguin : public Scene {
private:
    std::vector<Penguin*> penguins;
    std::function<void()> cancel_spawn;
public:
    MGScenePenguin();
    void s_update() override;
    void c_update() override;
    void s_setup() override;
    void c_setup() override;
    void connect_listeners() override;
    void disconnect_listeners() override;
    void reset_camera() override;
    void reset_scene() override;
    void start_timers() override;
    void stop_timers() override;
};