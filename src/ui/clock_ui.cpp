#include <events.h>
#include "clock_ui.h"

#include "timer.h"
#include "util/color.h"

ClockUI::ClockUI(float aspect, int rounds)
        : UI(40.f * aspect, 90.f),
          clock(format_time(0), // clock starts at 00:00
                0.f, 0.f, // starting coordinates
                20.f * aspect, 10.f, // width and height
                Color::WHITE, Color::BLACK, // foreground and background
                0.5f), // transparency
          round_count_bg(0, -2.f,
                20.f * aspect, 2.f, // width and height
                rounds, rounds, // num rectangles = num rounds
                (20.f * aspect - (rounds + 1) * 0.2f) / rounds, 2.f, // width and height
                Color::BLACK, // background color
                0.2f, // padding
                0) { // no halo selector

    attach(clock);

    float padding = 0.2f;
    float round_height = 2.f;
    float round_width = (20.f * aspect - (rounds + 1) * padding) / rounds;
	
    // Create a UI rectangle for each round. Will set color when that round is reached.
    for (int i = 0; i < rounds; ++i) {
        UIRectangle* round_seg = new UIRectangle(0, padding, // start coordinates
            round_width, round_height - 2 * padding, // width and height
            Color::CYAN, 0.2f); // rectangle color and alpha
        round_count_bg.attach_at(0, i, *round_seg);
        round_counts.push_back(round_seg);
    }

    attach(round_count_bg);
    disable();

    events::menu::end_menu_event.connect([&]() {
        enable();
    });

    events::ui::update_time.connect([&](int time) {
        set_time(time);
    });

    events::ui::round_changed_event.connect([&](int next_round) {
        // If we go past the max number of rounds, no change to UI.
        if (next_round <= round_counts.size())
            round_counts[next_round - 1]->set_alpha(0.8f);
    });
}

ClockUI::~ClockUI() {
    for (auto & rect : round_counts) {
        delete rect;
    }
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