#pragma once

#include "scene/scene.h"
#include "minigame.h"

/*
Holds the functionality for the "Pushy Penguins" minigame
*/

const auto PENGUINMG_TIME = std::chrono::seconds(30);
const int PENGUINMG_REWARD = 500;

class PenguinMG : public Minigame {
public:
    PenguinMG(Context& to_set);

    void s_setup() override;
    void s_teardown() override;
    void c_setup() override;
    void c_teardown() override;

    bool is_finished() override;
private: 
    boost::signals2::connection joystick_conn;
    boost::signals2::connection collision_conn;
    boost::signals2::connection flag_conn;

    std::map<int, bool> dead_player_flags;
};