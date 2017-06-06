#include <events.h>
#include "main_menu_ui.h"

#include "timer.h"
#include "util/color.h"
#include "asset_loader.h"

MainMenuUI::MainMenuUI(float aspect)
        : UI(0.f * aspect, 0.f),
          bg(0.f, 0.f,
             100.f * aspect, 100.f, Color::WHITE, 0.f),
          logo(20.f * aspect, 50.f,
               20.f * 2.9464f, 20.f,
               AssetLoader::get_texture("sandma.png")) {
    attach(bg);
    attach(logo);

    // Invisible to start off with.
    disable();
    logo.set_alpha(0.f);

    events::ui::enable_main_menu.connect([&](){
       enable_animated();
    });

    events::ui::disable_main_menu.connect([&]() {
        disable_animated();
    });
}

void MainMenuUI::enable_animated() {
    enable();
    logo.animate_alpha(1.f, 0.5f);
    bg.animate_alpha(0.8f, 0.5f);
}

void MainMenuUI::disable_animated() {
    logo.animate_alpha(0.f, 0.5f);
    // have bg disable the ui after it disappears
    bg.animate_alpha(0.f, 0.51f, [&]() {
        disable();
    });
}