#include <events.h>
#include <timer.h>
#include "countdown_ui.h"

#define START_Y -50.f

CountdownUI::CountdownUI(float aspect)
    : count(30.f, 30.f, 0.f, START_Y, 100.f * aspect, 100.f, 0.f,
            UIUnstretchedTextBox::MIDDLE, UIUnstretchedTextBox::START,
            Color::WHITE, Color::BLACK, 1.f, false)
    , cur_timer([](){}) {
    enable();

    // Hide at first
    count.set_alpha(0.f);
//    count.set_text("1");
    attach(count);

    events::ui::show_countdown.connect([this](const std::vector<std::string> &to_display, const std::function<void()> &do_after) {
        // Construct the lambdas
        auto execute = do_after;
        for (auto it = to_display.rbegin(); it != to_display.rend(); ++it)
            execute = make_counter(*it, execute);

        // IT'S GO TIME.
        execute();
    });
}

std::function<void()> CountdownUI::make_counter(const std::string &text, const std::function<void()> &do_after) {
    return [=]() {
        // Reset state
        cur_timer();
        count.animate_to(0.f, START_Y + 10.f, .01f);
        count.animate_alpha(0.f, .01f, [=]() {
            // Then animate downwards with text
            count.set_text(text);
            count.animate_to(0.f, START_Y, .25f);
            count.animate_alpha(1.f, .25f, [=]() {
                // Then wait a bit before fading out
                cur_timer = Timer::get()->do_after(std::chrono::milliseconds(300), [=]() {
                    count.animate_alpha(0.f, .25f, [=]() {
                        // Finally, wait a bit then do after
                        cur_timer = Timer::get()->do_after(std::chrono::milliseconds(200), do_after);
                    });
                });
            });
        });
    };
}
