#pragma once

#include "events.h"
#include "timer.h"

class Phase {
public:
    Phase(Context& context);
    virtual void s_setup() = 0;
    virtual proto::Phase s_update() { return next; };
    virtual void s_teardown() = 0;
    virtual void c_setup() = 0;
    virtual void c_update() {};
    virtual void c_teardown() = 0;

    // Used for transition text.
    virtual std::string to_string() = 0;

    // Used to query for the next logical phase.
    // We need this because the next phase changes in some rounds.
    virtual proto::Phase s_phase_when_done() = 0;
protected:
    Context& context;

    // If you must instigate a phase change outside of update() (e.g. via timed callback), do it here.
    proto::Phase next;

    // Whether or not to show tutorial phases
    bool show_dialogues;
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
    int remaining_countdown;

    // Use to set up timer to transition to next phase after specified time.
    void transition_after(int countdown, int seconds, proto::Phase to);
};
