#include "phase.h"

#include <iostream>

const auto BUILD_TIME = std::chrono::seconds(10);
const auto DUNGEON_TIME = std::chrono::seconds(10);

void TimedPhase::transition_after(const Timer::Duration &time, Phase *to) {
    cancel_clock_every = Timer::get()->do_every(std::chrono::seconds(1), [&](){
        static auto remaining = BUILD_TIME;
        remaining -= std::chrono::seconds(1);
        std::cerr << remaining.count() << std::endl;
        if (remaining <= std::chrono::seconds(0)) {
            cancel_clock_every();
            next = to;
        }
    });
}
