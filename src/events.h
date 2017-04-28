#pragma once

#include <boost/signals2.hpp>
#include "window.h"
#include "game/construct_types.h"

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

    namespace build {
        struct ConstructData {
            ConstructType type;
            int x;
            int z;
        };
        extern signal<void(ConstructType)> construct_changed_event;
        extern signal<void(ConstructData &)> try_build_event;
        extern signal<void(ConstructData &)> confirm_build_event;
    }
}
