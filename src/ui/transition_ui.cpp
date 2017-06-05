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
    initial_start_x(-25.f * aspect), initial_start_y(-25.f),
    bg(initial_start_x, initial_start_y, 150.f * aspect, 150.f, AssetLoader::get_texture("black_fade.png")),
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
    bg.set_start_x(initial_start_x);
    bg.set_start_y(initial_start_y);
}

void TransitionUI::wipe(float seconds, Color c, Direction dir, std::function<void()> do_apex) {
    //bg.set_color(c);
    bg.set_alpha(1.f);
    enable();

    // Calculate starting and ending positions of wipe.
    float new_start_x = initial_start_x;
    float new_start_y = initial_start_y;
    float target_x = initial_start_x;
    float target_y = initial_start_y;
    switch(dir) {
        case Direction::UP:
            new_start_y = -125.f;
            target_y = 100.f;
            break;
        case Direction::DOWN:
            new_start_y = 100.f;
            target_y = -125.f;
            break;
        case Direction::LEFT:
            new_start_x = 100.f * aspect;
            target_x = -125.f * aspect;
            break;
        case Direction::RIGHT:
            new_start_x = -125.f * aspect;
            target_x = 100.f * aspect;
            break;
        default:
            break;
    }

    // Set starting position of wipe.
    bg.set_start_x(new_start_x);
    bg.set_start_y(new_start_y);

    // Animate to cover the screen.
    bg.animate_to(initial_start_x, initial_start_y, seconds / 3.f, [&, do_apex, seconds, target_x, target_y]() {
        do_apex();
        Timer::get()->do_after(std::chrono::milliseconds((int) (seconds / 3.f * 1000)), [&, seconds, target_x, target_y]() {
            // Complete animation to target position.
            bg.animate_to(target_x, target_y, seconds / 3.f, [&]() {
                // 3 nested lambdas.
                wipe_reset();
            });
        });
    });
}