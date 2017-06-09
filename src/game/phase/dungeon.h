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
    std::string to_string() override;
    proto::Phase s_phase_when_done() override;
private:
    boost::signals2::connection joystick_conn;
    boost::signals2::connection button_conn;
    boost::signals2::connection collision_conn;
    boost::signals2::connection interact_conn;
    boost::signals2::connection flag_conn;
    boost::signals2::connection player_finish_conn;
    boost::signals2::connection essence_conn;

    // <Player id, order of completion>
    std::map<int, int> goal_reached_flags;

    // Counter for finish order
    int next_place;

    // Vector for points, the player's ranking is 
    // the index used for accessing the reward
    // (0th place indicates you didn't finish)
    std::vector<int> rewards =
    { 0, 100, 75, 50, 25 };
};
