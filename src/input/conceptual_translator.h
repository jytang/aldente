#pragma once
#include "events.h"
#include "raw_maps/raw_map.h"

namespace input {
// Translation unit for numeric joypad input to conceptual button.
class ConceptualTranslator {
public:
    // Sets up event listeners/dispatchers
    ConceptualTranslator(
            const std::map<RawButton, events::ConceptualButton> &joy_map,
            const std::map<RawKeyboard, events::ConceptualButton> &key_map
    );

    // Query last known state, defaults to 0
    // joystick parameter specifies which joystick, -1 is all joysticks.
    // NOTE: querying for individual joysticks (i.e. joystick != -1) is not implemented.
    int get_state(events::ConceptualButton button, int joystick = -1) const;

private:
    const std::map<RawButton, events::ConceptualButton> &joy_map;
    const std::map<RawKeyboard, events::ConceptualButton> &key_map;

    // Cached state for all joysticks
    std::map<events::ConceptualButton, int> cache;
};
}
