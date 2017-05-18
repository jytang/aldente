#include "phase.h"

#include <iostream>

void TimedPhase::transition_after(int seconds, proto::Phase to) {
    cancel_clock_every();
    next = proto::Phase::NOOP;
    remaining_seconds = seconds;
    cancel_clock_every = Timer::get()->do_every(std::chrono::seconds(1), [this, to](){
        remaining_seconds--;
        std::cerr << remaining_seconds << std::endl;
        if (remaining_seconds <= 0) {
            cancel_clock_every();
            next = to;
        }
    });
}
