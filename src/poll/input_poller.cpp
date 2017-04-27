#include "poll/input_poller.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "events.h"

void InputPoller::poll() {
    // Only controller 1 is supported.
    int count;

    static std::vector<unsigned char> p_btns;
    const unsigned char *btns = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
    p_btns.resize(static_cast<unsigned long>(count), GLFW_RELEASE);
    if (btns) {
        for (int i = 0; i < count; ++i) {
            const bool same = p_btns[i] == btns[i];
            p_btns[i] = btns[i];
            if (!same) {
                events::JoystickData d = {1, true, i, btns[i] == GLFW_PRESS};
                events::joystick_event(d);
            }
        }
    }

    static std::vector<int> p_axes;
    static std::vector<int> frames;
    static int sensitivity = 30;
    const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
    p_axes.resize(static_cast<unsigned long>(count), 0);
    frames.resize(static_cast<unsigned int>(count), sensitivity);
    if (axes) {
        for (int i = 0; i < count; ++i) {
            static float step = static_cast<float>(1.0 / events::INPUT_ANALOG_LEVELS);
            const int level = static_cast<int>(axes[i] / step);
            const bool same = p_axes[i] == level;
            p_axes[i] = level;
            if (same && --(frames[i]) == 0) {
                frames[i] = sensitivity;
                events::JoystickData d = {1, false, i, level};
                events::joystick_event(d);
            }
            if (!same) {
                frames[i] = sensitivity;
                events::JoystickData d = {1, false, i, level};
                events::joystick_event(d);
            }
        }
    }
}
