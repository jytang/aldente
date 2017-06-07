#include "events.h"

namespace events {
    using boost::signals2::signal;
    signal<void(const JoystickData &)> joystick_event;
    signal<void(const ButtonData &)> button_event;
    signal<void(const StickData &)> stick_event;
    signal<void(const AudioData &)> music_event;
    signal<void(const AudioData &)> sound_effects_event;
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
        signal<void()> toggle_fps_event;
        signal<void(int)> fps_count_event;
        signal<void()> ping_event;
        signal<void(long long)> ping_changed_event;
        signal<void()> increase_hdr_exposure_event;
        signal<void()> decrease_hdr_exposure_event;
        signal<void()> toggle_hdr_event;
        signal<void()> toggle_bloom_event;
    }

    signal<void(RigidBodyData d)> add_rigidbody_event;
    signal<void(GameObject *obj)> remove_rigidbody_event;
    signal<void(GameObject *obj)> disable_rigidbody_event;
    signal<void(GameObject *obj)> enable_rigidbody_event;
    signal<void(GameObject *obj)> update_collision_mask_event;
    signal<void(int)> player_finished_event;
    signal<void(const proto::PlayerStats &)> c_player_stats_updated;
    signal<void(int)> player_ready_event;

    signal<void(glm::vec3 position, int time, std::function<void()> do_after)> camera_anim_position_event;
    signal<void(glm::vec3 axis, float angle, int time, std::function<void()> do_after)> camera_anim_rotate_event;

    namespace server {
        signal<void(proto::ServerMessage &)> announce;
    }

    namespace client {
        signal<void(proto::ClientMessage &)> send;
    }

    namespace menu {
        signal<void(int)> request_join_event;
        signal<void(int, proto::JoinResponse &)> respond_join_event;
        signal<void(int, int)> spawn_existing_player_event;
        signal<void(bool)> c_cycle_player_model_event;
        signal<void()> end_menu_event;
    }

    namespace ui {
        signal<void(int)> update_time;
        signal<void()> toggle_leaderboard;
        signal<void()> disable_leaderboard;
        signal<void()> enable_leaderboard;
        signal<void(int, int, std::string)> leaderboard_update;
        signal<void(const std::vector<std::tuple<std::string, int, int>> &)> scoreboard_sequence;
        signal<void()> disable_scoreboard;
        signal<void(const std::vector<std::pair<std::string, std::string>> &)> show_dialog;
        signal<void(const std::string &)> show_notification;
        signal<void(float, std::string, std::function<void()>)> transition_wipe;
        signal<void(float, std::string, std::function<void()>)> transition_fade;
        signal<void()> enable_main_menu;
        signal<void()> disable_main_menu;
        signal<void(const std::vector<std::string> &, const std::function<void()> &)> show_countdown;
        signal<void(int)> round_changed_event;
    }

    namespace build {
        signal<void(Direction)> select_grid_move_event;
        signal<void(Direction)> build_grid_move_event;
        signal<void()> select_grid_confirm_event;
        signal<void()> build_grid_place_event;
        signal<void()> select_grid_return_event;
        signal<void(ConstructType)> construct_selected_event;
        signal<void(ConstructType, bool)> c_construct_preview_event;
        signal<void(bool)> c_rotate_preview_event;
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
    }

    namespace dungeon {
        signal<void(glm::vec3, glm::vec3, float, std::function<void(GameObject *bt_hit)>)> request_raycast_event;
        signal<void(glm::vec3)> player_position_updated_event;
        signal<void()> s_prepare_dungeon_event;
        signal<void(const StickData &)> network_player_move_event;
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

    namespace minigame {
        signal<void()> start_minigame_event;
        signal<void()> end_minigame_event;
        signal<void(int)> player_died_event;
    }
}
