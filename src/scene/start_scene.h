#pragma once

#include "scene/scene.h"
#include "asset_loader.h"
#include "game_objects/grid.h"
#include "game_objects/player.h"
#include "light/pulse_point_light.h"

/*
Starting scene to be used before the gameplay. Will hold things
like the logo screen and character selection.
*/
class StartScene : public Scene {
private:
    std::vector<GameObject*> platforms;
    Player* client_player;
    bool menu_enabled;
public:
    StartScene();
    void s_update() override;
    void c_update() override;
    void s_setup() override;
    void c_setup() override;
    void zoom_to_client_player(Player*);
};