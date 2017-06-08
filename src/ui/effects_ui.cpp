#include "effects_ui.h"

#include "events.h"
#include "asset_loader.h"
#include "timer.h"

EffectsUI::EffectsUI(float aspect)
    : UI(0, 0),
      image(0.f, 0.f, 100.f * aspect, 100.f, AssetLoader::get_texture("sandma.png")), // sandma placeholder
      rect(0.f, 0.f, 100.f * aspect, 100.f, Color::BLACK, 0.f),
      text_box(4.f, 4.f, 0.f, 0.f, 100.f * aspect, 100.f, 0.f,
            UIUnstretchedTextBox::MIDDLE, UIUnstretchedTextBox::MIDDLE,
            Color::BLACK, Color::BLACK, 1.f, false) {

    attach(image);
    attach(rect);
    attach(text_box);

    image.set_alpha(0.f);
    rect.set_alpha(0.f);
    text_box.set_alpha(0.f);

    // Turn off everything to begin with.
    image.disable();
    rect.disable();
    text_box.disable();

    // Attach event listeners
    events::ui::ice_effect.connect([&]() {
       show_image(1.f, "icy.png");
    });
}

void EffectsUI::show_image(float seconds, std::string image_name) {
    image.enable();
    image.set_image(AssetLoader::get_texture(image_name));
    image.animate_alpha(1.f, seconds / 4.f, [&, seconds]() {
        Timer::get()->do_after(std::chrono::milliseconds(static_cast<int>(seconds / 2.f * 1000)), [&, seconds]() {
           image.animate_alpha(0.f, seconds / 4.f, [&, seconds]() {
              image.disable();
           });
        });
    });
}

void EffectsUI::show_rect(float seconds, Color c) {
    rect.enable();
    rect.set_color(c);
    rect.animate_alpha(1.f, seconds / 4.f, [&, seconds]() {
        Timer::get()->do_after(std::chrono::milliseconds(static_cast<int>(seconds / 2.f * 1000)), [&, seconds]() {
            rect.animate_alpha(0.f, seconds / 4.f, [&, seconds]() {
                rect.disable();
            });
        });
    });
}

void EffectsUI::show_text(float seconds, std::string text) {
    text_box.enable();
    text_box.set_text(text);
    text_box.animate_alpha(1.f, seconds / 4.f, [&, seconds]() {
        Timer::get()->do_after(std::chrono::milliseconds(static_cast<int>(seconds / 2.f * 1000)), [&, seconds]() {
            text_box.animate_alpha(0.f, seconds / 4.f, [&, seconds]() {
                text_box.disable();
            });
        });
    });
}