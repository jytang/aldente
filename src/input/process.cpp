#include "input/process.h"

#include <GLFW/glfw3.h>
#include <vector>
#include "events/input.h"

namespace kuuhaku {
namespace input {

// Run to poll for input and dispatch events.
void process() {
  int count;

  static std::vector<unsigned char> p_btns;
  const unsigned char* btns = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
  p_btns.resize(count, GLFW_RELEASE);
  if (btns) {
    for (int i = 0; i < count; ++i) {
      const bool same = p_btns[i] == btns[i];
      p_btns[i] = btns[i];
      if (!same) {
        events::InputEvent.dispatch({
          .joystick = 1,
          .is_button = true,
          .which = i,
          .level = btns[i] == GLFW_PRESS,
        });
      }
    }
  }

  static std::vector<int> p_axes;
  const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
  p_axes.resize(count, 0);
  if (axes) {
    for (int i = 0; i < count; ++i) {
      static float step = 1.0 / events::INPUT_ANALOG_LEVELS;
      const int level = axes[i] / step;
      const bool same = p_axes[i] == level;
      p_axes[i] = level;
      if (!same) {
        events::InputEvent.dispatch({
          .joystick = 1,
          .is_button = false,
          .which = i,
          .level = level,
        });
      }
    }
  }
}

}
}
