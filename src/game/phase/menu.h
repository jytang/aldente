#pragma once

#include "phase.h"

class MenuPhase : public Phase {
public:
    MenuPhase(Context& context) : Phase(context) {};
    void s_setup() override;
    proto::Phase s_update() override;
    void s_teardown() override;
    void c_setup() override;
    void c_update() override;
    void c_teardown() override;
    std::string to_string() override;
    proto::Phase s_phase_when_done() override;
private:
    boost::signals2::connection ready_conn;
    boost::signals2::connection button_conn;

    bool ready_sound = true;
};
