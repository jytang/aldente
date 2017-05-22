#pragma once

#include "phase.h"

const auto DUNGEON_TIME = std::chrono::seconds(10);

class DungeonPhase : public TimedPhase {
public:
    DungeonPhase(Context& context) : TimedPhase(context) {};
    void s_setup() override;
    proto::Phase s_update() override;
    void s_teardown() override;
    void c_setup() override;
    void c_update() override;
    void c_teardown() override;
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
