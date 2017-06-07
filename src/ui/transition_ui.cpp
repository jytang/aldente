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
    container(initial_start_x, initial_start_y),
    bg(0, 0, 150.f * aspect, 150.f, AssetLoader::get_texture("black_fade.png")),
    title(4.f, 4.f, 25.f * aspect, -13.f, 90.f * aspect, 100.f, 0.f,
          UIUnstretchedTextBox::END, UIUnstretchedTextBox::MIDDLE,
          Color::BLACK, Color::BLACK, 1.f, false),
    tbc(72.f * aspect, 30.f, 4.4182f * 23.f, 23.f, AssetLoader::get_texture("tbc.png")),
    aspect(aspect) {

    disable();
    attach(container);
    container.attach(bg);
    container.attach(tbc);
    container.attach(title);

    events::ui::transition_wipe.connect([&](float seconds, std::string s, std::function<void()> do_apex) {
        title.set_text(s);
        wipe(seconds,
             Color::BLACK,
             Direction::RIGHT,
             do_apex);
    });

    events::ui::transition_fade.connect([&](float seconds, std::string s, std::function<void()> do_apex) {
        title.set_text(s);
        fade(seconds,
             Color::BLACK,
             do_apex);
    });
}

// Do a fade to black (or whatever color you pass in, and then fade back.
void TransitionUI::fade(float seconds, Color c, std::function<void()> do_apex) {
    container.set_alpha(0.f);
    enable();
    container.animate_alpha(1.f, seconds / 3.f, [&, seconds, do_apex]() {
        do_apex();
        Timer::get()->do_after(std::chrono::milliseconds((int) (seconds / 3.f * 1000)), [&, seconds]() {
            container.animate_alpha(0.f, seconds / 3.f, [&]() {
              disable();
           });
       });
    });
}

void TransitionUI::wipe_reset() {
    disable();
    container.set_start_x(initial_start_x);
    container.set_start_y(initial_start_y);
}

// Do a wipe transition, like Microsoft Powerpoint 2003.
void TransitionUI::wipe(float seconds, Color c, Direction dir, std::function<void()> do_apex) {
    container.set_alpha(1.f);
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
    container.set_start_x(new_start_x);
    container.set_start_y(new_start_y);

    // Animate to cover the screen.
    container.animate_to(initial_start_x, initial_start_y, seconds / 3.f, [&, do_apex, seconds, target_x, target_y]() {
        do_apex();
        Timer::get()->do_after(std::chrono::milliseconds((int) (seconds / 3.f * 1000)), [&, seconds, target_x, target_y]() {
            // Complete animation to target position.
            container.animate_to(target_x, target_y, seconds / 3.f, [&]() {
                // 3 nested lambdas.
                wipe_reset();
            });
        });
    });
}