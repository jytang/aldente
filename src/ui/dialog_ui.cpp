#include <asset_loader.h>
#include <util/util.h>
#include <audio/audio_manager.h>
#include "dialog_ui.h"
#include "timer.h"
#include "events.h"
#include "input/modal_input.h"

DialogUI::DialogUI(float aspect, float width, float height, float padding)
    : bg(0, 0, aspect * 100.f, 100.f, Color::BLACK, 0.5f)
    , textbox(3.f, 4.f,
              aspect * (100.f - width) / 2, // Center
              padding, aspect * width, height, padding,
              UIUnstretchedTextBox::START, UIUnstretchedTextBox::START, Color::WHITE, Color::BLACK, 0.5f)
    , portrait(aspect * (width + (100.f - width) / 2) - (height * 3), // Right-align
               0.f, height * 3, height * 3,
               AssetLoader::get_texture("dio.jpg"))
    , cancel_text_animation([](){})
    , animating(false) {

    // Don't show by default
    disable();
    attach(bg);
    attach(portrait);
    attach(textbox);

    events::ui::show_dialog.connect([&](const auto &sequence, const auto &do_after_) {
        do_after = do_after_;

        // Start displaying text
        enable();
        current_dialog = sequence;
        std::reverse(current_dialog.begin(), current_dialog.end()); // Reverse so we can pop_back
        display_next();

        // Switch to dialog bindings and recieve button presses
        const auto &mi = input::ModalInput::get();
        mi->set_mode(input::ModalInput::DIALOG);
        button_conn = mi->with_mode(input::ModalInput::DIALOG).buttons.connect([&](const events::ButtonData &d) {
            if (d.state == 0) return;
            switch (d.input) {
                case events::BTN_START:
                case events::BTN_A:
                    if (animating)
                        skip_animation();
                    else
                        display_next();
                    break;
                default:
                    break;
                }
        });
    });

}

void DialogUI::display_next() {
    events::sound_effects_event(events::AudioData(AudioManager::SELECT1_SOUND, false));

    // Reset the text box
    display = "";

    // If we had nothing remaining from before, bring in the next dialogue
    if (remaining.empty()) {
        // If no next, just stop
        if (current_dialog.empty()) {
            stop_display();
            return;
        }

        // Grab the next portrait and text
        std::string portrait_str;
        std::tie(portrait_str, remaining) = current_dialog.back();
        current_dialog.pop_back();

        // Set the portrait
        portrait.set_image(AssetLoader::get_texture(portrait_str));

    }

    // Chomp the next window-full of text
    std::tie(current, remaining) = Util::wordbreak_text(remaining, textbox.get_max_chars());

    // Start a text animation
    animating = true;
    cancel_text_animation();
    cancel_text_animation = Timer::get()->do_every(std::chrono::milliseconds(20), [&]() {
        display += current.substr(0, 1);
        current = current.substr(1);

        // hacky way to have dialogue sound per 4 characters.
        static int char_count = 0;
        char_count++;
        if (char_count % 4 == 0)
            events::sound_effects_event(events::AudioData(AudioManager::SELECT2_SOUND, false));

        textbox.set_text(display);

        if (current.empty()) {
            animating = false;
            cancel_text_animation();
        }
    });
}

void DialogUI::skip_animation() {
    events::sound_effects_event(events::AudioData(AudioManager::SELECT1_SOUND, false));

    animating = false;
    cancel_text_animation();
    textbox.set_text(display + current);
}

void DialogUI::stop_display() {
    // Restore modal input state and disable the UI
    button_conn.disconnect();
    input::ModalInput::get()->set_mode(input::ModalInput::NORMAL);
    disable();
    do_after();
}
