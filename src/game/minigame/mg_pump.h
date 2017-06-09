#pragma once

#include "scene/scene.h"
#include "minigame.h"

/*
Holds the functionality for the "Hot Air Balloon" minigame
*/

const auto PUMPMG_TIME = std::chrono::seconds(30);
const int PUMPMG_REWARD = 500;

class PumpMG : public Minigame {
public:
    PumpMG(Context& to_set);

    void s_setup() override;
    void s_teardown() override;
    void c_setup() override;
    void c_teardown() override;

    bool is_finished() override;
private:
    boost::signals2::connection inflate_conn;
    boost::signals2::connection button_conn;

    // Order: team 1 player 1, team 2 player 1,
    //        team 1 player 2, team 2 player 2
    std::vector<glm::vec3> player_start_pos{
        { -5, 0.0f, -0.5f },
        { 5, 0.0f, -0.5f },
        { -1, 0.0f, -0.5f },
        { 1, 0.0f, -0.5f },
    };

    std::vector<Player*> team1;
    std::vector<Player*> team2;
    int team1_points;
    int team2_points;
};