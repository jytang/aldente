#pragma once

#include <boost/signals2.hpp>
#include "window.h"
#include "proto/net.pb.h"
#include "game/construct_types.h"
#include "game/direction.h"

namespace events {

    using boost::signals2::signal;

    const int INPUT_ANALOG_LEVELS = 5;
    struct JoystickData {
        int id; // Which joystick
        bool is_button;
        int input; // Button or axis number
        int state; // If button, zero is not pressed, nonzero is pressed.
        // Otherwise, is axis analog level.
    };
    extern signal<void(JoystickData &)> joystick_event;

    struct WindowSizeData {
        Window *window;
        int width;
        int height;
    };
    extern signal<void(WindowSizeData &)> window_buffer_resize_event;

    struct WindowKeyData {
        Window *window;
        int key;
        int scancode;
        int action;
        int mods;
    };
    extern signal<void(WindowKeyData &)> window_key_event;

    struct WindowCursorData {
        Window *window;
        double x_pos;
        double y_pos;
    };
    extern signal<void(WindowCursorData &)> window_cursor_event;

    struct WindowMouseButtonData {
        Window *window;
        int button;
        int action;
        int mods;
    };
    extern signal<void(WindowMouseButtonData &)> window_mouse_button_event;

    struct WindowScrollData {
        Window *window;
        double x_off;
        double y_off;
    };
    extern signal<void(WindowScrollData &)> window_scroll_event;

    extern signal<void()> toggle_debug_shadows_event;
    extern signal<void()> toggle_ui_event;

    namespace build {        
        // Move the selection in the 2D selection grid.
        extern signal<void(Direction)> select_grid_move_event;

        // Move the selection in the 3D build grid.
        extern signal<void(Direction)> build_grid_move_event;

        // Confirm construct selection in the 2D selection grid.
        extern signal<void()> select_grid_confirm_event;

        // Confirm placement in the 3D build grid.
        extern signal<void()> build_grid_place_event;

        // The selected construct type has changed.
        extern signal<void(ConstructType)> construct_changed_event;

        // Client requests to build a construct.
        extern signal<void(proto::Construct &)> request_build_event;

        // Server attempts to build the construct.
        extern signal<void(proto::Construct &)> try_build_event;

        // Server responds whether or not the build was successful.
        extern signal<void(proto::Construct &)> respond_build_event;

        // Client updates local grid with newly built construct.
        extern signal<void(proto::Construct &)> update_build_event;
    }
}
