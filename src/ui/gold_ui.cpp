#include "gold_ui.h"

#include "events.h"
#include "timer.h"
#include "util/color.h"
#include "asset_loader.h"

#include <string>

GoldUI::GoldUI(float aspect)
        : UI(90.f * aspect, 0.f),
          gold_delta("+0g",
                     0.f, 10.f, // starting coordinates
                     5.f * aspect, 5.f, // width and height
                     Color::WHITE),
          total_gold("500g",
                     0.f, 5.f,
                     5.f * aspect, 5.f,
                     Color::WHITE),
          gold_image(5.f * aspect, 5.f,
                     5.f * aspect, 5.f, AssetLoader::get_texture("Tomato.jpg")) {

    attach(gold_delta);
    gold_delta.disable(); // not shown by default

    attach(total_gold);
    attach(gold_image);

    events::ui::update_time.connect([&](int time) {
        set_gold(time);
    });
}

void GoldUI::set_gold(int gold) {
    total_gold.set_text(std::to_string(gold) + "g");
}