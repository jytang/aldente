#include <events.h>
#include <input/modal_input.h>
#include "main_menu_ui.h"

#include "timer.h"
#include "util/color.h"
#include "asset_loader.h"
#include <iostream>

MainMenuUI::MainMenuUI(float aspect)
        : UI(0.f * aspect, 0.f),
          bg(0.f, 0.f,
             100.f * aspect, 100.f, Color::WHITE, 0.f),
          logo((100.f * aspect - 30.f * 2.9464f) / 2.f, 50.f,
               30.f * 2.9464f, 30.f,
               AssetLoader::get_texture("sandma.png")),
          press(5.f, 5.f, 0.f, -20.f, 100.f * aspect, 100.f, 0.f,
                UIUnstretchedTextBox::MIDDLE, UIUnstretchedTextBox::MIDDLE,
                Color::BLACK, Color::BLACK, 1.f, false),
          press_anim([](){}) {
    attach(bg);
    attach(logo);
    attach(press);

    // Invisible to start off with.
    disable();
    logo.set_alpha(0.f);

    // Press start
    press.set_text("Press START");

    events::ui::enable_main_menu.connect([&](){
        // Switch to main menu button bindings.
        const auto &mi = input::ModalInput::get();
        mi->set_mode(input::ModalInput::MAIN_MENU);
        button_conn = mi->with_mode(input::ModalInput::MAIN_MENU).buttons.connect([&](const events::ButtonData &d) {
            if (d.state == 0) return;
            switch (d.input) {
                case events::BTN_START:
                    events::ui::disable_main_menu();
                    break;
                default:
                    break;
            }
        });

        enable_animated();
    });

    events::ui::disable_main_menu.connect([&]() {
        // Restore modal input state and disable the UI
        button_conn.disconnect();
        input::ModalInput::get()->set_mode(input::ModalInput::NORMAL);

        disable_animated();
    });
}

void MainMenuUI::enable_animated() {
    enable();
    logo.animate_alpha(1.f, 0.5f);
    bg.animate_alpha(0.8f, 0.5f);

    // Recursively oscillate the opacity of press
    press_anim = [&](){
        std::cerr << "test" << std::endl;
        press.animate_alpha(1.f, 0.5f, [&](){
            press.animate_alpha(0.f, 0.5f, [&](){
                press_anim();
            });
        });
    };
    press_anim();
}

void MainMenuUI::disable_animated() {
    logo.animate_alpha(0.f, 0.5f);
    // have bg disable the ui after it disappears
    bg.animate_alpha(0.f, 0.51f, [&]() {
        disable();
    });
    // Stop the recursion!
    press_anim = [](){};
}