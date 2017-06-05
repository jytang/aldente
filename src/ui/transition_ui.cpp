#include "transition_ui.h"

#include "timer.h"
#include "events.h"
#include <stdlib.h>
#include <iostream>

#include "asset_loader.h"

std::vector<Color> TransitionUI::transition_colors = {
    Color::BLACK, Color::OCEAN_BLUE,
    Color::WINDWAKER_GREEN, Color::WINDWAKER_SAND,
    Color::AUTUMN_ORANGE, Color::OLIVE_GREEN, Color::PURPLE,
    Color::BONE_WHITE, Color::INDIAN_RED
};

TransitionUI::TransitionUI(float aspect) :
    UI(0, 0),
    bg2(0, 0, 100.f * aspect, 100.f, Color::BLACK, 1.f),
    bg(-25.f * aspect, -25.f, 150.f * aspect, 150.f, AssetLoader::get_texture("black_fade.png")),
    aspect(aspect) {

    disable();
    attach(bg);

    events::ui::transition_wipe.connect([&](float seconds, std::function<void()> do_apex) {
        wipe(seconds,
             Color::BLACK,
             static_cast<Direction>((rand() % 4)),
             do_apex);
    });

    events::ui::transition_fade.connect([&](float seconds, std::function<void()> do_apex) {
        fade(seconds,
             Color::BLACK,
             do_apex);
    });
}

void TransitionUI::fade(float seconds, Color c, std::function<void()> do_apex) {
    //bg.set_color(c);
    bg.set_alpha(0.f);
    enable();
    bg.animate_alpha(1.f, seconds / 3.f, [&, seconds, do_apex]() {
        do_apex();
        Timer::get()->do_after(std::chrono::milliseconds((int) (seconds / 3.f * 1000)), [&, seconds]() {
           bg.animate_alpha(0.f, seconds / 3.f, [&]() {
              disable();
           });
       });
    });
}

void TransitionUI::wipe_reset() {
    disable();
    bg.set_start_x(-25.f);
    bg.set_start_y(-25.f);
}

void TransitionUI::wipe(float seconds, Color c, Direction dir, std::function<void()> do_apex) {
    //bg.set_color(c);
    bg.set_alpha(1.f);
    enable();

    // Don't look at this.
    switch(dir) {
        // Because it's ugly.
        case Direction::UP:
            bg.set_start_x(-25.f * aspect);
            bg.set_start_y(100.f);
            bg.animate_to(-25.f * aspect, -25.f, seconds / 3.f, [&, do_apex, seconds]() {
                do_apex();
                Timer::get()->do_after(std::chrono::milliseconds((int) (seconds / 3.f * 1000)), [&, seconds]() {
                    bg.animate_to(-25.f * aspect, -125.f, seconds / 3.f, [&]() {
                        // 3 nested lambdas.
                        wipe_reset();
                    });
                });
            });
            break;
        case Direction::DOWN:
            bg.set_start_x(-25.f * aspect);
            bg.set_start_y(-125.f);
            bg.animate_to(-25.f * aspect, -25.f, seconds / 3.f, [&, do_apex, seconds]() {
                do_apex();
                Timer::get()->do_after(std::chrono::milliseconds((int) (seconds / 3.f * 1000)), [&, seconds]() {
                    bg.animate_to(-25.f * aspect, 100.f, seconds / 3.f, [&]() {
                        wipe_reset();
                    });
                });
            });
            break;
        case Direction::LEFT:
            bg.set_start_y(-25.f);
            bg.set_start_x(-125.f * aspect);
            bg.animate_to(-25.f * aspect, -25.f, seconds / 3.f, [&, do_apex, seconds]() {
                do_apex();
                Timer::get()->do_after(std::chrono::milliseconds((int) (seconds / 3.f * 1000)), [&, seconds]() {
                    bg.animate_to(100.f * aspect, -25.f, seconds / 3.f, [&]() {
                        wipe_reset();
                    });
                });
            });
            break;
        case Direction::RIGHT:
            bg.set_start_y(-25.f);
            bg.set_start_x(100.f * aspect);
            bg.animate_to(-25.f * aspect, -25.f, seconds / 3.f, [&, do_apex, seconds]() {
                do_apex();
                Timer::get()->do_after(std::chrono::milliseconds((int) (seconds / 3.f * 1000)), [&, seconds]() {
                    bg.animate_to(-125.f * aspect, -25.f, seconds / 3.f, [&]() {
                        wipe_reset();
                    });
                });
            });
            break;
        default:
            break;
    }
}