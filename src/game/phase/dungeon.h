#pragma once

#include "phase.h"

class DungeonPhase : public TimedPhase {
public:
    void setup();
    void teardown();
private:
    boost::signals2::connection joystick_conn;
    boost::signals2::connection button_conn;
};