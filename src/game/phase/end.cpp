#include "end.h"

EndPhase::EndPhase(Context &context)
    : Phase(context) {}

void EndPhase::s_setup() {
    next = s_phase_when_done();
}

void EndPhase::s_teardown() {
}

void EndPhase::c_setup() {
}

void EndPhase::c_update() {
}

void EndPhase::c_teardown() {
}

std::string EndPhase::to_string() {
    return "Awakening";
}

proto::Phase EndPhase::s_phase_when_done() {
    return proto::Phase::MENU;
}
