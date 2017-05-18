#pragma once

#include "events.h"
#include "timer.h"

class Phase {
public:
    Phase(Context& context) : context(context), next(proto::Phase::NOOP) {};
    virtual void setup() = 0;
    virtual proto::Phase update() { return next; };
    virtual void teardown() = 0;
    virtual void client_setup() = 0;
    virtual void client_update() {};
    virtual void client_teardown() = 0;
    virtual std::map<int, bool> get_flags() { return std::map<int, bool>(); };
protected:
    Context& context;
    proto::Phase next;
};

class TimedPhase : public Phase {
public:
	TimedPhase(Context& context)
            : Phase(context)
            , cancel_clock_every([]{})
            , remaining_seconds(0) {}
protected:
    std::function<void()> cancel_clock_every;
    int remaining_seconds;

    // Use to set up timer to transition to next phase after specified time.
    void transition_after(int seconds, proto::Phase to);
};
