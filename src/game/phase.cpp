#include "phase.h"

PhaseType Phase::curr_phase = PhaseType::BUILD;

bool BuildPhase::is_menu = true;

void BuildPhase::update() {
    std::cerr << "BuildPhase::update()\n";
}
