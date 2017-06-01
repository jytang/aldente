#include "ui_element.h"

#include "timer.h"

#include <cmath> // std::abs
#include <iostream>

#define UI_ANIMATION_STEP_MS 17 // 60fps

// Helper to determine if the next destination is farther away from the target
bool UIElement::moving_away(float current, float delta, float target) {
    float curr_dist = std::abs(current - target);
    float next_dist = std::abs(current + delta - target);
    return curr_dist <= next_dist;
}

void UIElement::animate_to(float target_x, float target_y, float seconds, std::function<void()> do_after) {
    // Calculate the amount to step by per UI_ANIMATION_STEP_MS.
    float milliseconds = seconds * 1000;
    float step_x = (target_x - start_x) / milliseconds * UI_ANIMATION_STEP_MS;
    float step_y = (target_y - start_y) / milliseconds * UI_ANIMATION_STEP_MS;

    // Cancel existing animation if any.
    cancel_animation();

    // Do animation every UI_ANIMATION_STEP_MS
    cancel_animation = Timer::get()->do_every(
            std::chrono::milliseconds(UI_ANIMATION_STEP_MS),
            [&, step_x, step_y, target_x, target_y, do_after]() {
                start_x += step_x;
                start_y += step_y;

                // Cancel animation once we are moving away from target.
                if (moving_away(start_x, step_x, target_x) &&
                        moving_away(start_y, step_y, target_y)) {
                    start_x = target_x;
                    start_y = target_y;
                    cancel_animation();
                    do_after();
                }
            });
}

void UIElement::animate_relative(float delta_x, float delta_y, float seconds, std::function<void()> do_after) {
    animate_to(start_x + delta_x, start_y + delta_y, seconds, do_after);
}