#pragma once

#include "raw_map.h"
#include "events.h"

namespace input {
    const std::map<RawButton, events::ConceptualButton> BTN_MAP_MATRICOM {
            // D-Pad
            {{true, 13}, events::BTN_UP},
            {{true, 16}, events::BTN_LEFT},
            {{true, 15}, events::BTN_DOWN},
            {{true, 14}, events::BTN_RIGHT},

            // ABXY
            {{true, 1}, events::BTN_A},
            {{true, 2}, events::BTN_B},
            {{true, 0}, events::BTN_X},
            {{true, 3}, events::BTN_Y},

            // Shoulders
            {{true, 6}, events::AX_LT},
            {{true, 4}, events::BTN_LB},
            {{true, 5}, events::BTN_RB},
            {{true, 7}, events::AX_RT},

            // Stick presses
            {{true, 10}, events::BTN_LS},
            {{true, 11}, events::BTN_RS},

            // Left stick
            {{false, 0}, events::AX_LH},
            {{false, 1}, events::AX_LV},

            // Right stick
            {{false, 2}, events::AX_RH},
            {{false, 3}, events::AX_RV},

            // Back, start
            {{true, 8}, events::BTN_BACK},
            {{true, 9}, events::BTN_START},

            // Xbox button
            {{true, 12}, events::BTN_XBOX},
    };
}
