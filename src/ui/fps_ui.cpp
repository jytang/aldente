#include "fps_ui.h"

#include "events.h"

FPSUI::FPSUI(float aspect)
    : UI(95.f * aspect, 95.f),
      fps_display("0",
                  0, 0,
                  5.f * aspect, 5.f,
                  Color::WHITE, Color::BLACK, 0.5f) {

    attach(fps_display);

    // not shown by default
    disable();

    events::debug::toggle_fps_event.connect([&]() {
        toggle();
    });

    events::debug::fps_count_event.connect([&](int fps) {
        fps_display.set_text(std::to_string(fps));
    });
}