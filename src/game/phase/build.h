#pragma once

#include "phase.h"

const auto BUILD_TIME = std::chrono::seconds(10);

class BuildPhase : public TimedPhase {
public:
    void setup();
    void teardown();
    static bool is_menu;
private:
    boost::signals2::connection joystick_conn;
    boost::signals2::connection button_conn;
};
