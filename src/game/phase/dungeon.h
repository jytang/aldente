#pragma once

#include "phase.h"

const auto DUNGEON_TIME = std::chrono::seconds(10);

class DungeonPhase : public TimedPhase {
public:
    DungeonPhase(Context& context) : TimedPhase(context) {};
    void setup();
    proto::Phase update() override;
    void teardown();
    void client_setup();
    void client_update();
    void client_teardown();
    std::map<int, bool> get_flags() override { return goal_reached_flags; };

private:
    boost::signals2::connection joystick_conn;
    boost::signals2::connection button_conn;
    boost::signals2::connection collision_conn;
    boost::signals2::connection interact_conn;
    boost::signals2::connection flag_conn;

    // <Player id, if goal reached>
    std::map<int, bool> goal_reached_flags;
};
