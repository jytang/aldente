#pragma once

#include "scene/scene.h"
#include "asset_loader.h"
#include "game_objects/player.h"
#include "light/pulse_point_light.h"

/*
End scene for declaring winner
*/
class EndScene : public Scene {
private:
    std::vector<GameObject*> platforms;
    Player *placeholder;
    std::function<void()> cancel_timer;

public:
    EndScene();
    void s_update() override;
    void c_update() override;
    void s_setup() override;
    void c_setup() override;
};