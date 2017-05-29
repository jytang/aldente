#pragma once

#include "../../scene/scene.h"
#include "minigame.h"

/*
Holds the functionality for the "Pushy Penguins" minigame
*/

const auto PENGUINMG_TIME = std::chrono::seconds(30);

class PenguinMG : public Minigame {
public:
    PenguinMG(Context& to_set);

    // Used to control listeners of the minigame
    void setup() override;
    void teardown() override;

    // To be called by minigame phase to determine
    // if the minigame should be terminated early
    bool is_finished() override;
private: 
    boost::signals2::connection joystick_conn;
    boost::signals2::connection collision_conn;
    boost::signals2::connection flag_conn;

    std::vector<Player*, bool> dead_player_flags;
};