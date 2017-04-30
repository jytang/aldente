#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "raw_map.h"
#include "events.h"

namespace input {
    const std::map<RawKeyboard, events::ConceptualButton> KBD_MAP_DEBUG {
            // Shift-WASD for D-Pad
            {{GLFW_KEY_W, GLFW_MOD_SHIFT, 1}, events::BTN_UP},
            {{GLFW_KEY_A, GLFW_MOD_SHIFT, 1}, events::BTN_LEFT},
            {{GLFW_KEY_S, GLFW_MOD_SHIFT, 1}, events::BTN_DOWN},
            {{GLFW_KEY_D, GLFW_MOD_SHIFT, 1}, events::BTN_RIGHT},

            // HJKL for ABXY
            {{GLFW_KEY_H, 0, 1}, events::BTN_A},
            {{GLFW_KEY_J, 0, 1}, events::BTN_B},
            {{GLFW_KEY_K, 0, 1}, events::BTN_X},
            {{GLFW_KEY_L, 0, 1}, events::BTN_Y},

            // YU for left shoulders, IO for right shoulders
            {{GLFW_KEY_Y, 0, events::INPUT_ANALOG_LEVELS}, events::AX_LT},
            {{GLFW_KEY_U, 0, 1}, events::BTN_LB},
            {{GLFW_KEY_I, 0, 1}, events::BTN_RB},
            {{GLFW_KEY_O, 0, events::INPUT_ANALOG_LEVELS}, events::AX_RT},

            // WASD for left stick
            {{GLFW_KEY_W, 0, -events::INPUT_ANALOG_LEVELS}, events::AX_LV},
            {{GLFW_KEY_A, 0, -events::INPUT_ANALOG_LEVELS}, events::AX_LH},
            {{GLFW_KEY_S, 0, events::INPUT_ANALOG_LEVELS}, events::AX_LV},
            {{GLFW_KEY_D, 0, events::INPUT_ANALOG_LEVELS}, events::AX_LH},

            // Arrow keys for right stick
            {{GLFW_KEY_UP, 0, -events::INPUT_ANALOG_LEVELS}, events::AX_RV},
            {{GLFW_KEY_LEFT, 0, -events::INPUT_ANALOG_LEVELS}, events::AX_RH},
            {{GLFW_KEY_DOWN, 0, events::INPUT_ANALOG_LEVELS}, events::AX_RV},
            {{GLFW_KEY_RIGHT, 0, events::INPUT_ANALOG_LEVELS}, events::AX_RH},

            // Brackets for back and start
            {{GLFW_KEY_LEFT_BRACKET, 0, 1}, events::BTN_BACK},
            {{GLFW_KEY_RIGHT_BRACKET, 0, 1}, events::BTN_START},

            // Enter for Xbox button
            {{GLFW_KEY_ENTER, 0, 1}, events::BTN_XBOX},
    };
}
