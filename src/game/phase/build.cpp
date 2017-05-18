#include <game/game_state.h>
#include "build.h"

bool BuildPhase::is_menu = true;

void BuildPhase::setup() {
    transition_after(10, proto::Phase::DUNGEON);
}

void BuildPhase::client_setup() {
    events::build::start_build_event();

    joystick_conn = events::stick_event.connect([&](events::StickData d) {

        // Left stick for UI Grid Movement
        if (d.input == events::STICK_LEFT) {
            // Movement axes.
            Direction dir;
            bool moving = true;
            if (d.state.first == events::INPUT_ANALOG_LEVELS)
                dir = Direction::RIGHT;
            else if (d.state.first == -events::INPUT_ANALOG_LEVELS)
                dir = Direction::LEFT;
            else if (d.state.second == events::INPUT_ANALOG_LEVELS)
                dir = Direction::DOWN;
            else if (d.state.second == -events::INPUT_ANALOG_LEVELS)
                dir = Direction::UP;
            else
                moving = false;

            if (moving) {
                if (is_menu)
                    events::build::select_grid_move_event(dir);
                else
                    events::build::build_grid_move_event(dir);
            }
        }
        else if (d.input == events::STICK_RIGHT) {
            events::build::pan_camera_event(d.state);
        }
    });

    button_conn = events::button_event.connect([&](events::ButtonData d) {
        // A button pressed.
        if (d.input == events::BTN_A && d.state == 1) {
            if (is_menu)
                events::build::select_grid_confirm_event();
            else
                events::build::build_grid_place_event();
            is_menu = false;
        }
        // B button pressed.
        else if (d.input == events::BTN_B && d.state == 1) {
            events::build::select_grid_return_event();
            is_menu = true;
        }
    });
}

void BuildPhase::teardown() {
    cancel_clock_every();
}

void BuildPhase::client_teardown() {
    joystick_conn.disconnect();
    button_conn.disconnect();

    events::build::end_build_event();
}
