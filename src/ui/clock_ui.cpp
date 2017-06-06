#include <events.h>
#include "clock_ui.h"

#include "timer.h"
#include "ui/ui_grid.h"
#include "util/color.h"
#include "util/config.h"

ClockUI::ClockUI(float aspect)
        : UI(40.f * aspect, 90.f),
          clock(format_time(0), // clock starts at 00:00
                0.f, 0.f, // starting coordinates
                20.f * aspect, 10.f, // width and height
                Color::WHITE, Color::BLACK, // foreground and background
                0.5f) { // transparency

    attach(clock);

    int rounds;
    Config::config->get_value(Config::str_num_rounds, rounds);
    std::cerr << "rounds: " << rounds << "\n";

    UIGrid* grid = new UIGrid(0, -5,
        20.f * aspect, 5.f, // width and height
        rounds, rounds,
        3, 3,
        Color::PURPLE,
        0.75f,
        0); /* ,
        float inter_padding = 0.75f,
        float selection_halo_padding = 0.3f, // hardcoded defaults :(
        float grid_bg_alpha = 1.f)*/
    attach(*grid);
    disable();

    events::menu::end_menu_event.connect([&]() {
        enable();
    });

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