#pragma once

#include "phase.h"

const auto BUILD_TIME = std::chrono::seconds(10);

class BuildPhase : public TimedPhase {
public:
    BuildPhase(Context& context) : TimedPhase(context) {};
    void s_setup() override;
    void s_teardown() override;
    proto::Phase s_update() override;
    void c_setup() override;
    void c_teardown() override;
    std::string to_string() override;
    proto::Phase s_phase_when_done() override;
private:
    static bool is_menu;
    boost::signals2::connection joystick_conn;
    boost::signals2::connection button_conn;
    boost::signals2::connection ready_conn;
    boost::signals2::connection s_verify_and_build_conn;
    boost::signals2::connection c_check_funds_conn;
    boost::signals2::connection c_preview_conn;
};
