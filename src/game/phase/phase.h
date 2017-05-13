#pragma once

#include "events.h"
#include "timer.h"

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
