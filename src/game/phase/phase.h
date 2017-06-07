#pragma once

#include "events.h"
#include "timer.h"

class Phase {
public:
    Phase(Context& context) : context(context), next(proto::Phase::NOOP) {};
    virtual void s_setup() = 0;
    virtual proto::Phase s_update() { return next; };
    virtual void s_teardown() = 0;
    virtual void c_setup() = 0;
    virtual void c_update() {};
    virtual void c_teardown() = 0;
    virtual std::string to_string() = 0;
protected:
    Context& context;

    // If you must instigate a phase change outside of update() (e.g. via timed callback), do it here.
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
