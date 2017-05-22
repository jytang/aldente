#include "clock_ui.h"

#include "timer.h"
#include "util/color.h"

ClockUI::ClockUI(float aspect)
    : clock(40.f * aspect, 90.f, 20.f * aspect, 10.f, Color::WHITE, Color::BLACK) {

    // TEST. REMOVE ME.
    attach(clock);
    Timer::get()->do_every(std::chrono::seconds(1), [&]() {
        static int seconds = 0;
        ++seconds;
        clock.set_time(seconds);
    });
}


