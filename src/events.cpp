#include "events.h"

namespace events {
    using boost::signals2::signal;
    signal<void(JoystickData &)> joystick_event;
    signal<void(ButtonData &)> button_event;
    signal<void(StickData &)> stick_event;
    signal<void(AudioData &)> music_event;
    signal<void(AudioData &)> sound_effects_event;
    signal<void()> toggle_mute_event;
    signal<void(WindowSizeData &)> window_buffer_resize_event;
    signal<void(WindowKeyData &)> window_key_event;
    signal<void(WindowCursorData &)> window_cursor_event;
    signal<void(WindowMouseButtonData &)> window_mouse_button_event;
    signal<void(WindowScrollData &)> window_scroll_event;

    namespace debug {
        signal<void()> toggle_debug_shadows_event;
        signal<void()> toggle_ui_event;
        signal<void()> toggle_light_rotation_event;
        signal<void()> toggle_debug_input_event;
        signal<void(Phase*)> client_set_phase_event;
        signal<void()> toggle_bt_debug_drawer_event;
        signal<void()> toggle_ui_text_box_background_event;
    }

    signal<void(RigidBodyData d)> add_rigidbody_event;
    signal<void(GameObject *obj)> remove_rigidbody_event;
    signal<void(GameObject *obj)> disable_rigidbody_event;
    signal<void(GameObject *obj)> enable_rigidbody_event;
    signal<void(int)> player_finished_event;
    signal<void(const proto::PlayerStats &)> c_player_stats_updated;

    namespace server {
        signal<void(proto::ServerMessage &)> announce;
    }

    namespace client {
        signal<void(proto::ClientMessage &)> send;
    }

    namespace menu {
        signal<void(int)> request_join_event;
        signal<void(int, proto::JoinResponse &)> respond_join_event;
        signal<void(int)> spawn_existing_player_event;
    }

    namespace ui {
        signal<void(int)> update_time;
    }

    namespace build {
        signal<void(Direction)> select_grid_move_event;
        signal<void(Direction)> build_grid_move_event;
        signal<void()> select_grid_confirm_event;
        signal<void()> build_grid_place_event;
        signal<void()> select_grid_return_event;
        signal<void(ConstructType)> construct_selected_event;
        signal<void(ConstructType, bool)> c_construct_preview_event;
        signal<void(proto::Construct &)> request_build_event;
        signal<void(ConstructType)> c_check_funds_event;
        signal<void(proto::Construct &)> s_verify_and_build;
        signal<void(proto::Construct &, std::function<void()>)> s_try_build_event;
        signal<void(proto::Construct &)> respond_build_event;
        signal<void(proto::Construct &)> update_build_event;
        signal<void(std::pair<int, int>)> pan_camera_event;
        signal<void(int,int)> hover_position_updated_event;
        signal<void()> start_build_event;
        signal<void()> end_build_event;
        signal<void(int)> player_ready_event;
    }

    namespace dungeon {
        signal<void(glm::vec3, glm::vec3, std::function<void(GameObject *bt_hit)>)> player_request_raycast_event;
        signal<void(glm::vec3)> player_position_updated_event;
        signal<void()> s_prepare_dungeon_event;
        signal<void(StickData &)> network_player_move_event;
        signal<void(Context*)> update_state_event;
        signal<void(int, int)> network_collision_event;
        signal<void()> player_interact_event;
        signal<void(int, int)> network_interact_event;
        signal<void(int, int, int)> spawn_existing_goal_event;
        signal<void(int)> player_finished_event;
        signal<void()> post_dungeon_camera_event;
        signal<void(float, float)> s_spawn_essence_event;
        signal<void(float, float, int)> c_spawn_essence_event;
    }
}
