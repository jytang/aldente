#pragma once

#include <boost/signals2.hpp>
#include "window.h"
#include "proto/net.pb.h"
#include "game/construct_types.h"
#include "game/direction.h"
#include "game/context.h"
#include "btBulletDynamicsCommon.h"

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
    extern signal<void(JoystickData &)> joystick_event;

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
    extern signal<void(ButtonData &)> button_event;

    // Stick
    enum Stick {
        STICK_LEFT, STICK_RIGHT
    };
    struct StickData {
        int id; // Which controller
        Stick input;
        std::pair<int, int> state;
    };
    extern signal<void(StickData &)> stick_event;

    // Audio
    struct AudioData {
        std::string filename;
    };
    extern signal<void(AudioData &)> music_event;
    extern signal<void(AudioData &)> sound_effects_event;

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
    }

    // The user has made a selection on the UI grid.
    extern signal<void(int)> ui_grid_selection_event;
    extern signal<void(int)> ui_grid_movement_event;

    // Struct for parameters for rigidbody initialization
    struct RigidBodyData {
        glm::vec3 position;
        float mass;
        btCollisionShape *shape;
        glm::vec3 inertia;

        //Object that has the rigidBody
        GameObject *object;
        // whether this rigidbody is a ghost/trigger
        bool is_ghost;
    };
    extern signal<void(RigidBodyData d)> add_rigidbody_event;
    extern signal<void(GameObject *obj)> remove_rigidbody_event;

    namespace server {
        extern signal<void(proto::ServerMessage &)> announce;
    }

    namespace menu {
        extern signal<void(int)> request_join_event;
        extern signal<void(int, proto::JoinResponse &)> respond_join_event;
        extern signal<void(int)> spawn_existing_player_event;
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

        // Client requests to build a construct.
        extern signal<void(proto::Construct &)> request_build_event;

        // Server attempts to build the construct.
        extern signal<void(proto::Construct &)> try_build_event;

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
    }

    namespace dungeon {
        // Player class asks physics for a raycast check
        extern signal<void(glm::vec3, glm::vec3,std::function<void(GameObject *bt_hit)>)> player_request_raycast_event;

        // Sends out signal for player's position. Used for camera to follow player
        extern signal<void(glm::vec3)> player_position_updated_event;

        // Signals goal placement
        extern signal<void()> place_goal_event;

        // Client requests the server to move player, passing its input stick data.
        extern signal<void(StickData &)> network_player_move_event;

        // Server sends context containing position, collisions, and interactions of game objects to all clients.
        extern signal<void(Context*)> update_state_event;

        // Server notifying clients that a collision occurred with game object of given id.
        extern signal<void(int)> network_collision_event;

        // Player interact (e.g opening a chest)
        extern signal<void()> player_interact_event;

        // Send an interaction over the network.
        extern signal<void(int)> network_interact_event;
        
        // Spawn an existing goal object on the client, at the given position.
        extern signal<void(int, int, int)> spawn_existing_goal_event;

        // Player reached the goal
        extern signal<void(int)> player_finished_event;
    }
}
