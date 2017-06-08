#include "phase.h"

#include <iostream>

void TimedPhase::transition_after(int countdown, int seconds, proto::Phase to) {
    cancel_clock_every();
    next = proto::Phase::NOOP;
    remaining_seconds = seconds;
    remaining_countdown = countdown;

    cancel_clock_every = Timer::get()->do_every(std::chrono::seconds(1), [this, to](){
        static proto::ServerMessage msg;
        remaining_countdown--;

        // Announce the time to clients if done with countdown
        if (remaining_countdown <= 0) {
            remaining_seconds--;
            msg.set_time_update(remaining_seconds);
            events::server::announce(msg);
        }

        if (remaining_seconds <= 0) {
            cancel_clock_every();
            next = to;
        }
    });
}
