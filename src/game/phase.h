#pragma once

#include "events.h"
#include "construct_types.h"
#include <iostream>

enum PhaseType {
    MENU, BUILD, DUNGEON, MINIGAME
};

class Phase {
public:
    virtual void update() = 0;
    static PhaseType curr_phase;
private:
    static std::unordered_map<PhaseType, Phase*> phases;
};

class BuildPhase : public Phase {
public:
    void update();
    static bool is_menu;
};