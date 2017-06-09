#pragma once

#include "phase.h"

class EndPhase : public Phase {
public:
    EndPhase(Context &contxt);
    void s_setup() override;
    void s_teardown() override;
    void c_setup() override;
    void c_update() override;
    void c_teardown() override;
    std::string to_string() override;
    proto::Phase s_phase_when_done() override;
};
