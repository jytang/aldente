#include "phase.h"

bool BuildPhase::is_menu = true;

void BuildPhase::setup() {
    joystick_conn = events::joystick_event.connect([&](events::JoystickData d) {
        // A button pressed.
        if (d.is_button == true && d.input == 0 && d.state == 0) {
            if (is_menu)
                events::build::select_grid_confirm_event();
            else
                events::build::build_grid_place_event();
            is_menu = false;
        }
        // B button pressed.
        else if (d.is_button == true && d.input == 1 && d.state == 0) {
            events::build::select_grid_return_event();
            is_menu = true;
        }

        // Movement axes.
        Direction dir;
        bool moving = false;
        if (d.is_button == 0 && d.input == 0) {
            moving = true;
            if (d.state == 5)
                dir = Direction::RIGHT;
            else if (d.state == -5)
                dir = Direction::LEFT;
            else
                moving = false;

        }
        else if (d.is_button == 0 && d.input == 1) {
            moving = true;
            if (d.state == 5)
                dir = Direction::DOWN;
            else if (d.state == -5)
                dir = Direction::UP;
            else
                moving = false;
        }

        if (moving) {
            if (is_menu)
                events::build::select_grid_move_event(dir);
            else
                events::build::build_grid_move_event(dir);
        }
    });
}

Phase* BuildPhase::update() {
    return nullptr;
}

void BuildPhase::teardown() {
    joystick_conn.disconnect();
}
