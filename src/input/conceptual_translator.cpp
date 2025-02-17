#include "conceptual_translator.h"

namespace input {
    ConceptualTranslator::ConceptualTranslator(
            const std::map<RawButton, events::ConceptualButton> &joy_map_,
            const std::map<RawKeyboard, events::ConceptualButton> &key_map_
    ) : joy_map(joy_map_), key_map(key_map_) {
        // Any window will trigger buttons.
        events::window_key_event.connect([&](const events::WindowKeyData &d) {
            // Ignore repeats
            if (d.action == GLFW_REPEAT) return;

            const auto &found = key_map.find({d.key, d.mods});
            // Bail if not in mapping
            if (found == key_map.end()) return;

            // Determine level to emit
            const int level = found->first.level;
            const int coeff = d.action == GLFW_RELEASE ? 0 : 1;

            // Cache the input
            const events::ConceptualButton button = found->second;
            const int state = level * coeff;
            cache[button] = state;

            // Emit the event with controller ID 0
            events::ButtonData next_d = {0, button, state};
            events::button_event(next_d);
        });

        events::joystick_event.connect([&](const events::JoystickData &d) {
            const auto &found = joy_map.find({d.is_button, d.input});
            // Bail if not in mapping
            if (found == joy_map.end()) return;

            // Cache the input
            const events::ConceptualButton button = found->second;
            cache[button] = d.state;

            // Emit the event with the controller
            events::ButtonData next_d = {d.id, button, d.state};
            events::button_event(next_d);
        });
    }

    int ConceptualTranslator::get_state(events::ConceptualButton button, int joystick) const {
        const auto &found = cache.find(button);
        return found != cache.end() ? found->second : 0;
    }
}
