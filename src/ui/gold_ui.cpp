#include "gold_ui.h"

#include "events.h"
#include "timer.h"
#include "util/color.h"
#include "asset_loader.h"

#include <string>

GoldUI::GoldUI(float aspect)
        : UI(90.f * aspect, 0.f),
          bg(0, 0, 10.f * aspect, 7.f * aspect, Color::BLACK, 0.5f),
          gold_delta(3.f, 4.f,
                     0, 0, 10.f * aspect, 8.f * aspect, // Same-ish size as BG for convenience
                     0.f, UIUnstretchedTextBox::MIDDLE, UIUnstretchedTextBox::START,
                     Color::WHITE, Color::BLACK, 0.f, false),
          gold_image(2.5f * aspect, -1.f,
                     5.f * aspect, 5.f * aspect,
                     AssetLoader::get_texture("essence.png")),
          total_gold("-",
                     2.5f * aspect, 4.f * aspect,
                     5.f * aspect, 5.f,
                     Color::WHITE),
          cur_gold(0),
          cur_delta(0),
          delta_showing(false),
          cancel_delta_out([](){}) {

    attach(bg);

    attach(total_gold);
    attach(gold_image);

    // Hide at first
    gold_delta.set_alpha(0.f);
    attach(gold_delta);
    set_gold(100); // TODO(metakirby5/jytang): don't hardcode heh

    events::c_player_stats_updated.connect([&](const proto::PlayerStats &update) {
        set_gold(update.coins());
    });
}

void GoldUI::set_gold(int gold) {
    // If delta not showing, animate it in and prepare to animate it out
    if (!delta_showing) {
        delta_showing = true;
        cur_delta = 0;
        gold_delta.animate_alpha(1.f, .25f);
        gold_delta.animate_relative(0, 2.f, .25f, [this]() {
            fade_delta_after(500);
        });
    } else {
        fade_delta_after(500);
    }

    // Update amounts
    cur_delta += gold - cur_gold;
    cur_gold = gold;
    total_gold.set_text(std::to_string(cur_gold));

    // Update delta text
    std::string prefix = cur_delta < 0 ? "-" : "+";
    Color color = cur_delta < 0 ? Color::INDIAN_RED : Color::WINDWAKER_GREEN;
    gold_delta.set_text(prefix + std::to_string(std::abs(cur_delta)));
    gold_delta.set_color(color);
}

void GoldUI::fade_delta_after(int ms) {
    // Cancel any existing fade state
    cancel_delta_out();

    // To cancel the existing animation, if any
    gold_delta.animate_alpha(1.f, .01f, [this]() {
        cancel_delta_out = Timer::get()->do_after(std::chrono::milliseconds(500), [this]() {
            gold_delta.animate_alpha(0.f, .1f, [this]() {
                // Reset the delta
                delta_showing = false;
                gold_delta.set_start_x(0);
                gold_delta.set_start_y(0);
            });
        });
    });
}