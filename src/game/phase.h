#pragma once

#include "events.h"
#include "construct_types.h"
#include <iostream>

class Phase {
public:
    virtual void setup() = 0;
    virtual Phase* update() = 0;
    virtual void teardown() = 0;
};

class MenuPhase : public Phase {
public:
    void setup() {}
    Phase* update() { return nullptr; }
    void teardown() {}
private:
};

class BuildPhase : public Phase {
public:
    void setup();
    Phase* update();
    void teardown();
    static bool is_menu;
private:
    boost::signals2::connection joystick_conn;
};

class DungeonPhase : public Phase {
public:
    void setup() {}
    Phase* update() { return nullptr; }
    void teardown() {}
private:
};

class MinigamePhase : public Phase {
public:
    void setup() {}
    Phase* update() { return nullptr; }
    void teardown() {}
private:
};
