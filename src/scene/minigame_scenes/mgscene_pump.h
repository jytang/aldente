#pragma once

#include "scene/scene.h"
#include "game_objects/minigame/pump.h"
#include "game_objects/minigame/hotair_basket.h"
#include "game_objects/minigame/hotair_balloon.h"
#include <map>

/*
Scene for pump minigame
*/
class MGScenePump : public Scene {
private:
    std::vector<Pump*> pumps;
    std::map<int, Pump*> pump_map;
    std::vector<HotAirBasket*> baskets;
    std::vector<HotAirBalloon*> balloons;
    int player_count;
public:
    MGScenePump();
    void s_update() override;
    void c_update() override;
    void s_setup() override;
    void c_setup() override;
    void connect_listeners() override;
    void disconnect_listeners() override;
    void reset_camera() override;
    void reset_scene() override;
    void inflate_balloon(bool is_team1);
};