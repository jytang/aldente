#pragma once

#include "events.h"
#include "construct_types.h"
#include <iostream>

enum PhaseType {
    MENU, BUILD, DUNGEON, MINIGAME
};

class Phase {
public:
    virtual void go() {}
};

class BuildPhase : public Phase {
public:
    void go() override {}
    BuildPhase() : is_menu(true) {}
    bool is_menu;
};