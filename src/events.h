#pragma once

#include <boost/signals2.hpp>
#include "window.h"
#include "proto/net.pb.h"
#include "game/construct_types.h"
#include "game/direction.h"
#include "game_objects/game_object.h"
#include "btBulletDynamicsCommon.h"

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
    }
    // The user has made a selection on the UI grid.
    extern signal<void(int)> ui_grid_selection_event;

    // Struct for parameters for rigidbody initialization
    struct RigidBodyData {
        glm::vec3 position;
        float mass;
        btCollisionShape *shape;
        glm::vec3 inertia;

        //Object that has the rigidBody
        GameObject *object;
        // whether this rigidbody is a ghost/trigger
        bool trigger;
    };
    extern signal<void(RigidBodyData d)> add_rigidbody_event;
    extern signal<void(GameObject *obj)> remove_rigidbody_event;

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

    namespace dungeon {
        // Player movement
        extern signal<void(StickData d)> player_move_event;

        // Player interact (e.g opening a chest)
        extern signal<void()> player_interact_event;

        // Player class asks physics for a raycast check
        extern signal<void(glm::vec3, glm::vec3,std::function<void(GameObject *bt_hit)>)> player_request_raycast_event;

    }
}
