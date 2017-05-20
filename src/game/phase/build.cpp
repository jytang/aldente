#include <game/game_state.h>
#include "build.h"
#include "audio/audio_manager.h"

bool BuildPhase::is_menu = true;

void BuildPhase::setup() {
//    transition_after(10, proto::Phase::DUNGEON);
}

void BuildPhase::client_setup() {
    events::build::start_build_event();
    events::build::select_grid_return_event();
    is_menu = true;

    joystick_conn = events::stick_event.connect([&](events::StickData d) {

        // Left stick for UI Grid Movement
        if (d.input == events::STICK_LEFT) {
            // Movement axes.
            Direction dir;
            bool moving = true;

            if (d.state.first == events::INPUT_ANALOG_LEVELS && d.state.second == 0)
                dir = Direction::RIGHT;
            else if (d.state.first == -events::INPUT_ANALOG_LEVELS && d.state.second == 0)
                dir = Direction::LEFT;
            else if (d.state.second == events::INPUT_ANALOG_LEVELS && d.state.first == 0)
                dir = Direction::DOWN;
            else if (d.state.second == -events::INPUT_ANALOG_LEVELS && d.state.first == 0)
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
        Direction dir;
        bool d_pad = false;

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
            if (!is_menu) {
                events::build::select_grid_return_event();
                is_menu = true;
            }
        }

        // D-Pad-Up pressed.
        else if (d.input == events::BTN_UP && d.state == 1) {
            dir = Direction::UP;
            d_pad = true;
        }

        // D-Pad-Right pressed.
        else if (d.input == events::BTN_RIGHT && d.state == 1) {
            dir = Direction::RIGHT;
            d_pad = true;
        }

        // D-Pad-Down pressed.
        else if (d.input == events::BTN_DOWN && d.state == 1) {
            dir = Direction::DOWN;
            d_pad = true;
        }

        // D-Pad-Left pressed.
        else if (d.input == events::BTN_LEFT && d.state == 1) {
            dir = Direction::LEFT;
            d_pad = true;
        }

        if (d_pad) {
            if (is_menu)
                events::build::select_grid_move_event(dir);
            else
                events::build::build_grid_move_event(dir);
        }
    });

    // Play music
    events::AudioData d = { AudioManager::BUILD_MUSIC };
    events::music_event(d);
}

void BuildPhase::teardown() {
    cancel_clock_every();
}

void BuildPhase::client_teardown() {
    joystick_conn.disconnect();
    button_conn.disconnect();

    events::build::end_build_event();
}
