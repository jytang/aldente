#pragma once

#include "scene/scene.h"
#include "minigame.h"

/*
Holds the functionality for the "Bumper Balls" minigame
*/

const auto SUMOMG_TIME = std::chrono::seconds(60);
const int SUMOMG_REWARD = 500;

class SumoMG : public Minigame {
public:
    SumoMG(Context& to_set);

    void s_setup() override;
    void s_teardown() override;
    void c_setup() override;
    void c_teardown() override;

    bool is_finished() override;
private:
    boost::signals2::connection joystick_conn;
    boost::signals2::connection collision_conn;
    boost::signals2::connection flag_conn;
    boost::signals2::connection button_conn;

    std::map<int, bool> dead_player_flags;

    std::vector<glm::vec3> player_start_pos{
        {-3, 0.8f, -3},
        {3, 0.8f, -3},
        {-3, 0.8f, 3},
        {3, 0.8f, 3},
    };
};