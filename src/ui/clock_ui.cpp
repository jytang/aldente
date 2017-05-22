#include <events.h>
#include "clock_ui.h"

#include "timer.h"
#include "util/color.h"

ClockUI::ClockUI(float aspect)
        : clock(format_time(0), // clock starts at 00:00
          40.f * aspect, 90.f, // starting coordinates
          20.f * aspect, 10.f, // width and height
          Color::WHITE, Color::BLACK, // foreground and background
          0.5f) { // transparency

    attach(clock);

    events::ui::update_time.connect([&](int time) {
        set_time(time);
    });
}

void ClockUI::set_time(int seconds) {
    clock.set_text(format_time(seconds));
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