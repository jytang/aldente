#pragma once
#include "events.h"
#include "raw_maps/raw_map.h"

namespace input {
// Translation unit for numeric joypad input to conceptual button.
class NumToButton {
public:
    // Sets up event listeners/dispatchers
    NumToButton(
            const std::map<RawButton, events::ConceptualButton> &joy_map,
            const std::map<RawKeyboard, events::ConceptualButton> &key_map
    );

private:
    const std::map<RawButton, events::ConceptualButton> &joy_map;
    const std::map<RawKeyboard, events::ConceptualButton> &key_map;
};
}
