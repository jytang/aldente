#pragma once

#include "phase.h"
#include <unordered_map>

class GameState {
public:
    // Adds a mapping of phase id to a Phase instance.
    static void add_phase(PhaseType type, Phase* phase);

    // Sets the current phase type.
    static void set_phase(PhaseType type);

    // Advances to the next phase. Since there's linear progression,
    // this just cycles through the phases.
    static void advance_phase();

    // Gets the current phase type.
    static PhaseType get_phase_type();

    // Gets the current phase instance.
    static Phase* get_phase();

private:
    static PhaseType current_phase;
    static std::unordered_map<PhaseType, Phase*> phases;
};