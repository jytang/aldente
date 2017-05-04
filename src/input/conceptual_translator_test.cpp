#include "catch.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "conceptual_translator.h"
#include "raw_maps/debug.h"
#include "raw_maps/xbox.h"
#include "event.h"

TEST_CASE("ConceptualTranslator", "input") {
    GIVEN("A ConceptualTranslator with a keyboard map") {
        const std::map<input::RawButton, events::ConceptualButton> btn;
        const std::map<input::RawKeyboard, events::ConceptualButton> kbd {
                {{GLFW_KEY_A, 0, 1}, events::BTN_A},
                {{GLFW_KEY_B, 0, 1}, events::BTN_B},
        };
        input::ConceptualTranslator translator(btn, kbd);

        WHEN("getting state on an unpressed button, it should be 0") {
            REQUIRE(translator.get_state(events::BTN_A) == 0);
            REQUIRE(translator.get_state(events::BTN_B) == 0);
        }

        WHEN("getting state on a pressed button, it should be the last state") {
            events::WindowKeyData d;

            d = {nullptr, GLFW_KEY_A, 0, GLFW_PRESS, 0};
            events::window_key_event(d);
            REQUIRE(translator.get_state(events::BTN_A) == 1);

            d = {nullptr, GLFW_KEY_A, 0, GLFW_RELEASE, 0};
            events::window_key_event(d);
            REQUIRE(translator.get_state(events::BTN_A) == 0);
        }
    }
}
