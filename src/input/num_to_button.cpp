#include "num_to_button.h"

namespace input {
    NumToButton::NumToButton(
            const std::map<RawButton, events::ConceptualButton> &joy_map_,
            const std::map<RawKeyboard, events::ConceptualButton> &key_map_
    ) : joy_map(joy_map_), key_map(key_map_) {
        events::window_key_event.connect([&](const events::WindowKeyData &d) {
            // Ignore repeats
            if (d.action == GLFW_REPEAT) return;

            const auto &found = key_map.find({d.key, d.mods});
            // Bail if not in mapping
            if (found == key_map.end()) return;

            // Determine level to emit
            const int level = found->first.level;
            const int coeff = d.action == GLFW_RELEASE ? 0 : 1;

            // Emit the event with controller ID 0
            events::ButtonData next_d = {0, found->second, level * coeff};
            events::button_event(next_d);
        });
    }
}
