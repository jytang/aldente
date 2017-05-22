#include <events.h>
#include "clock_ui.h"

#include "timer.h"
#include "util/color.h"

ClockUI::ClockUI(float aspect) {
    // Clock starts at 00:00.
    // DYNAMICALLY ALLOCATED TO FIX BUG WITH CHILDREN NOT BEING REAL.
    clock = new UITextBox(40.f * aspect, 90.f, 20.f * aspect, 10.f,
                      format_time(0),
                      Color::WHITE, Color::BLACK,
                      0.5f);
    attach(*clock);

    // TEST. REMOVE ME.
    Timer::get()->do_every(std::chrono::seconds(1), [&]() {
        static int seconds = 0;
        ++seconds;
        set_time(seconds);
    });
}

void ClockUI::set_time(int seconds) {
    clock->set_text(format_time(seconds));
}

std::string ClockUI::format_time(int seconds) {
    int hours = seconds / 3600;
    seconds %= 3600;
    int minutes = seconds / 60;
    seconds %= 60;

    char buf[128];

    if (hours == 0)
        sprintf(buf, "%02d:%02d", minutes, seconds);
    else
        sprintf(buf, "%02d:%02d:%02d", hours, minutes, seconds);
    return buf;
}