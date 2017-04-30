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
    signal<void()> toggle_ui_event;
    signal<void(int)> ui_grid_selection_event;

    namespace build {
        signal<void(Direction)> select_grid_move_event;
        signal<void(Direction)> build_grid_move_event;
        signal<void()> select_grid_confirm_event;
        signal<void()> build_grid_place_event;
        signal<void()> select_grid_return_event;
        signal<void(ConstructType)> construct_changed_event;
        signal<void(proto::Construct &)> request_build_event;
        signal<void(proto::Construct &)> try_build_event;
        signal<void(proto::Construct &)> respond_build_event;
        signal<void(proto::Construct &)> update_build_event;
    }

    namespace dungeon {
        signal<void(Direction)> player_move_event;
        signal<void()> player_interact_event;
        signal<void(glm::vec3 position, glm::vec3 dir)> player_request_raycast_event;
        signal<void(GameObject *bt_hit)> player_raycast_response_event;
    }
}
