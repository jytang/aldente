#pragma once

#include "events.h"
#include "timer.h"
#include "construct_types.h"
#include <iostream>

class Phase {
public:
    Phase() : next(nullptr) {}
    virtual void setup() = 0;
    virtual Phase* update() { return next; }
    virtual void teardown() = 0;

protected:
    Phase *next;
};

class TimedPhase : public Phase {
protected:
    std::function<void()> cancel_clock_every;

    // Use to set up timer to transition to next phase after specified time.
    void transition_after(const Timer::Duration &time, Phase *const to);
};

class MenuPhase : public Phase {
public:
    void setup() {}
    Phase* update() { return nullptr; }
    void teardown() {}
private:
};

class BuildPhase : public TimedPhase {
public:
    void setup();
    void teardown();
    static bool is_menu;
private:
    boost::signals2::connection joystick_conn;
    boost::signals2::connection button_conn;
};

class DungeonPhase : public TimedPhase {
public:
    void setup();
    void teardown();
private:
    boost::signals2::connection joystick_conn;
    boost::signals2::connection button_conn;
};

class MinigamePhase : public TimedPhase {
public:
    void setup() {}
    void teardown() {}
private:
};
