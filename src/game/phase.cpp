#include "phase.h"

bool BuildPhase::is_menu = true;

void BuildPhase::setup() {
    joystick_conn = events::joystick_event.connect([&](events::JoystickData d) {
        // A button pressed.
        if (d.is_button == true && d.input == 0 && d.state == 0) {
            events::build::grid_placement_event(is_menu);
            is_menu = false;
        }
        // B button pressed.
        else if (d.is_button == true && d.input == 1 && d.state == 0) {
            events::build::construct_changed_event(ConstructType::NONE);
            is_menu = true;
        }

        // Movement axes.
        if (d.is_button == 0 && d.input == 0) {
            if (d.state == 5)
                events::build::grid_move_event(Direction::RIGHT, is_menu);
            if (d.state == -5)
                events::build::grid_move_event(Direction::LEFT, is_menu);
        }
        else if (d.is_button == 0 && d.input == 1) {
            if (d.state == 5)
                events::build::grid_move_event(Direction::DOWN, is_menu);
            if (d.state == -5)
                events::build::grid_move_event(Direction::UP, is_menu);
        }
    });
}

Phase* BuildPhase::update() {
    return nullptr;
}

void BuildPhase::teardown() {
    joystick_conn.disconnect();
}
