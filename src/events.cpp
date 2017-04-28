#include "events.h"

namespace events {
    using boost::signals2::signal;
    signal<void(JoystickData &)> joystick_event;
    signal<void(WindowSizeData &)> window_buffer_resize_event;
    signal<void(WindowKeyData &)> window_key_event;
    signal<void(WindowCursorData &)> window_cursor_event;
    signal<void(WindowMouseButtonData &)> window_mouse_button_event;
    signal<void(WindowScrollData &)> window_scroll_event;
    signal<void()> toggle_debug_shadows_event;

    namespace build {
        signal<void(ConstructType)> construct_changed_event;
        signal<void(ConstructData &)> request_build_event;
        signal<void(ConstructData &)> try_build_event;
        signal<void(ConstructData &, bool)> respond_build_event;
        signal<void(ConstructData &)> update_build_event;
    }
}
