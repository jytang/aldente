#include "axis_combiner.h"

using events::Stick;
using events::ConceptualButton;

namespace input {
    AxisCombiner::AxisCombiner(const ConceptualTranslator &translator_, const StickMap &sticks_)
            : translator(translator_), sticks(sticks_) {
        // Record which stick corresponds to which axis
        for (const auto& p : sticks) {
            Stick s;
            std::pair<ConceptualButton, ConceptualButton> axs;
            ConceptualButton x, y;

            std::tie(s, axs) = p;
            std::tie(x, y) = axs;
            axis_to_stick[x] = s;
            axis_to_stick[y] = s;
        }

        // Emit stick_events in response to button_events
        events::button_event.connect([&](const events::ButtonData &d) {
            const auto &stick_found = axis_to_stick.find(d.input);

            // Bail if not interested in the button
            if (stick_found == axis_to_stick.end()) return;

            // Get the axes we want to request in the order required
            const Stick &stick = stick_found->second;
            const auto &axes_found = sticks.find(stick);
            assert(axes_found != sticks.end());
            ConceptualButton x, y;
            std::tie(x, y) = axes_found->second;

            // Request each axis state and dispatch the event
            events::StickData next_d = {
                    d.id, stick, {
                            translator.get_state(x, d.id),
                            translator.get_state(y, d.id)
                    }
            };
            events::stick_event(next_d);
        });
    }
}
