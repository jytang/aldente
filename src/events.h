#pragma once

#include <boost/signals2.hpp>
#include "window.h"
#include "proto/net.pb.h"
#include "game/construct_types.h"
#include "game/direction.h"
#include "game/context.h"
#include "btBulletDynamicsCommon.h"
#include "collision_groups.h"

// Forward declaration to resolve circular dependency.
class GameObject;
class Player;
class Phase;

namespace events {

    using boost::signals2::signal;

    const int INPUT_ANALOG_LEVELS = 5;

    // Raw joystick input
    struct JoystickData {
        int id; // Which joystick
        bool is_button;
        int input; // Button or axis number
        int state; // If button, zero is not pressed, nonzero is pressed.
        // Otherwise, is axis analog level.
    };
    extern signal<void(const JoystickData &)> joystick_event;

    // Conceptual button input
    enum ConceptualButton {
        BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT, // D-pad
        BTN_A, BTN_B, BTN_X, BTN_Y, // Face buttons
        BTN_BACK, BTN_XBOX, BTN_START, // Middle buttons
        BTN_LB, BTN_RB, // Bumpers
        BTN_LS, BTN_RS, // Stick presses

        AX_LV, AX_LH, AX_RV, AX_RH, // Left and right sticks, vertical and horizontal axes
        AX_LT, AX_RT, // Left and right triggers
    };
    struct ButtonData {
        int id; // Which controller
        ConceptualButton input;
        int state; // If button, zero is not pressed, nonzero is pressed.
        // Otherwise, is axis analog level.
    };
    extern signal<void(const ButtonData &)> button_event;

    // Stick
    enum Stick {
        STICK_LEFT, STICK_RIGHT
    };
    struct StickData {
        int id; // Which controller
        Stick input;
        std::pair<int, int> state;
    };
    extern signal<void(const StickData &)> stick_event;

    // Audio
    struct AudioData {
        std::string filename;
        bool loop;
        float distance = 0.0;  // for spatial sound effects

        //For global music or sound effects
        AudioData(std::string f, bool l) : filename(f), loop(l) {}

        //For spatial sound effects
        AudioData(std::string f, bool l, GameObject * p, GameObject * o) : filename(f), loop(l) {
            assert(p);
            assert(o);
            distance = glm::distance(p->transform.get_position(), o->transform.get_position());
        }
    };
    extern signal<void(const AudioData &)> music_event;
    extern signal<void(const AudioData &)> sound_effects_event;
    extern signal<void()> stop_all_sounds;
    extern signal<void()> toggle_mute_event;

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

    namespace debug {
        extern signal<void()> toggle_debug_shadows_event;
        extern signal<void()> toggle_ui_event;
        extern signal<void()> toggle_light_rotation_event;
        extern signal<void()> toggle_debug_input_event;
        extern signal<void(Phase*)> client_set_phase_event;
        extern signal<void()> toggle_bt_debug_drawer_event;
        extern signal<void()> toggle_ui_text_box_background_event;
        extern signal<void()> toggle_fps_event;
        extern signal<void(int)> fps_count_event;
        extern signal<void()> ping_event;
        extern signal<void(long long)> ping_changed_event;
        extern signal<void()> increase_hdr_exposure_event;
        extern signal<void()> decrease_hdr_exposure_event;
        extern signal<void()> toggle_hdr_event;
        extern signal<void()> toggle_bloom_event;
        extern signal<void()> print_camera_event;
    }

    // Struct for parameters for rigidbody initialization
    struct RigidBodyData {
        GameObject *object = nullptr; //Object that has the rigidBody
        btCollisionShape *shape = new btEmptyShape(); // Collider Shape
        bool is_ghost = false; // whether this rigidbody is a ghost/trigger
        float mass = 0.0f;
        glm::vec3 inertia = glm::vec3(0.0f);
        glm::vec3 position = glm::vec3(0.0f); // Initial position
    };

    extern signal<void(RigidBodyData d)> add_rigidbody_event;
    extern signal<void(GameObject *obj)> remove_rigidbody_event;
    extern signal<void(GameObject *obj)> disable_rigidbody_event;
    extern signal<void(GameObject *obj)> enable_rigidbody_event;
    extern signal<void(GameObject *obj)> update_collision_mask_event;
    extern signal<void(int)> player_finished_event;

    // Client-side. Called when the client player's gold amount has updated.
    extern signal<void(const proto::PlayerStats &)> c_player_stats_updated;

    // Signals that we should toggle player's "ready" status.
    extern signal<void(int)> player_ready_event;

    /* Events for camera animation. Call on client
    * Position: The end position of the transition
    * Angle: The total amount of rotation desired (degrees)
    * Axis: The axis of rotation
    * Time: Amount of time the transition should take (milliseconds)
    */
    extern signal<void(glm::vec3 position, int time, std::function<void()> do_after)> camera_anim_position_event;
    extern signal<void(glm::vec3 axis, float angle, int time, std::function<void()> do_after)> camera_anim_rotate_event;

    namespace server {
        extern signal<void(proto::ServerMessage &)> announce;
    }

    namespace client {
        extern signal<void(proto::ClientMessage &)> send;
    }

    namespace menu {
        extern signal<void(int)> request_join_event;
        extern signal<void(int, proto::JoinResponse &)> respond_join_event;

        // Spawns a player with given obj id and model index.
        extern signal<void(int, int)> spawn_existing_player_event;

        // Cycles to the next player avatar model.
        // Bool direction decides if we're cycling forward or backward.
        extern signal<void(bool)> c_cycle_player_model_event;

        // Signals the end of menu phase.
        extern signal<void()> end_menu_event;
    }

    namespace ui {
        // Update the UI timer.
        extern signal<void(int)> update_time;
        extern signal<void()> toggle_leaderboard;
        extern signal<void()> disable_leaderboard;
        extern signal<void()> enable_leaderboard;
        extern signal<void(int, int, std::string)> leaderboard_update;
        // Display scoreboard with vector of <model_name, gold, gold_delta>
        extern signal<void(const std::vector<std::tuple<std::string, int, int>> &)> scoreboard_sequence;
        extern signal<void()> disable_scoreboard;
        extern signal<void(float, std::string, std::function<void()>)> transition_wipe;
        extern signal<void(float, std::string, std::function<void()>)> transition_fade;
        extern signal<void()> enable_main_menu;
        extern signal<void()> disable_main_menu;
        extern signal<void(float, std::string)> show_effect_image;
        extern signal<void(float)> hide_effect_image;

        // Request some dialog to be shown
        // Show a sequence of dialog with events::ui::show_dialog({{portrait_str, text}, ...}).
        extern signal<void(const std::vector<std::pair<std::string, std::string>> &, const std::function<void()> &)> show_dialog;

        // Create a notification
        extern signal<void(const std::string &)> show_notification;

        // Display a countdown
        // Parmeters are (strings_to_show, do_after_callback)
        extern signal<void(const std::vector<std::string> &, const std::function<void()> &)> show_countdown;
        
        // Indicates that the current round (numerical) has changed; updates UI.
        extern signal<void(int)> round_changed_event;

        // Sets and shows a legend
        extern signal<void(const std::vector<std::pair<std::string, std::string>> &)> show_legend;

        // Dismisses legend
        extern signal<void()> dismiss_legend;
    }

    namespace dialogue {
        // Player finished dialogue
        extern signal<void(int)> s_player_finished_dialogue;
    }

    namespace build {
        // Move the selection in the 2D selection grid.
        extern signal<void(Direction)> select_grid_move_event;

        // Move the selection in the 3D build grid.
        extern signal<void(Direction)> build_grid_move_event;

        // Confirm construct selection in the 2D selection grid.
        extern signal<void()> select_grid_confirm_event;

        // Confirm placement in the 3D build grid.
        extern signal<void()> build_grid_place_event;

        // Return to the select grid from the place grid.
        extern signal<void()> select_grid_return_event;

        // The selected construct type has changed.
        extern signal<void(ConstructType)> construct_selected_event;

        // Show the construct preview on the 3D build grid.
        // Tint green if valid bool is true, otherwise tint red.
        extern signal<void(ConstructType, bool)> c_construct_preview_event;

        // Rotate the preview, True: cw, False: ccw
        extern signal<void(bool)> c_rotate_preview_event;

        // Client requests to build a construct.
        extern signal<void(proto::Construct &)> request_build_event;

        // Check if the player has enough funds to purchase, if so, allows selection of item.
        // Provide the type of the construct to build.
        extern signal<void(ConstructType)> c_check_funds_event;

        // Check if the player has enough funds to purchase, if so, tries to build.
        // Provide the construct proto object (containing player id) to build.
        extern signal<void(proto::Construct &)> s_verify_and_build;

        // Server attempts to build the construct.
        // Calls the given success function if the build succeeded.
        extern signal<void(proto::Construct &, std::function<void()>)> s_try_build_event;

        // Server responds whether or not the build was successful.
        extern signal<void(proto::Construct &)> respond_build_event;

        // Client updates local grid with newly built construct.
        extern signal<void(proto::Construct &)> update_build_event;

        // Pans camera during build phase
        extern signal<void(std::pair<int, int>)> pan_camera_event;

        // Camera follows hover 
        extern signal<void(int,int)> hover_position_updated_event;

        // Signals that the build phase has started
        extern signal<void()> start_build_event;

        // Signals that the build phase has ended
        extern signal<void()> end_build_event;

        // Updates the UI to indicate that the user has readied (or unreadied) up.
        extern signal<void()> toggle_ui_ready_event;
    }

    namespace dungeon {
        // Client dungeon phase start
        extern signal<void()> c_start;

        // Player class asks physics for a raycast check
        extern signal<void(glm::vec3 pos, glm::vec3 dir, float dist, std::function<void(GameObject *bt_hit)>)> request_raycast_event;

        // Sends out signal for player's position. Used for camera to follow player
        extern signal<void(glm::vec3)> player_position_updated_event;

        // Signals goal placement
        extern signal<void()> s_prepare_dungeon_event;

        // Client requests the server to move player, passing its input stick data.
        extern signal<void(const StickData &)> network_player_move_event;

        // Server sends context containing position, collisions, and interactions of game objects to all clients.
        extern signal<void(Context*)> update_state_event;

        // Server notifying clients that a collision occurred with game object of given ids.
        // Convention for ID order is (dispatcher, other).
        extern signal<void(int, int)> network_collision_event;

        // Player interact (e.g opening a chest)
        extern signal<void()> player_interact_event;

        // Send an interaction over the network.
        // Convention for ID order is (dispatcher, other).
        extern signal<void(int, int)> network_interact_event;
        
        // Spawn an existing goal object on the client, at the given position.
        extern signal<void(int, int, int)> spawn_existing_goal_event;

        // Player reached the goal
        extern signal<void(int)> player_finished_event;

        // Sets camera to post-dungeon camera
        extern signal<void()> post_dungeon_camera_event;
        
        // Spawns a Dream Essence object
        extern signal<void(float, float)> s_spawn_essence_event; // Server
        extern signal<void(float, float, int)> c_spawn_essence_event; // Client
    }

    namespace minigame {
        // Signals the start of the minigame phase, on both server and client.
        extern signal<void()> start_minigame_event;

        // Signals the end of the minigame phase, on both server and client.
        extern signal<void()> end_minigame_event;

        // Signals that the player has died
        extern signal<void(int)> player_died_event;
    }
}
