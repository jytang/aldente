#pragma once

#include "phase.h"

const auto DUNGEON_TIME = std::chrono::seconds(10);

class DungeonPhase : public TimedPhase {
public:
    void setup();
    void teardown();
private:
    boost::signals2::connection joystick_conn;
    boost::signals2::connection button_conn;
};