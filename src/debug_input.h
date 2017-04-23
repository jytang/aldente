#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


/*
	Handles GLFW calls for getting Mouse and Keyboard Input (also temporarily holds Camera for debugging)
*/
class DebugInput {
private:
    DebugInput();

    static bool keys[1024];
    static bool lmb_down;
    static bool rmb_down;
    static bool mouse_moved;
    static glm::vec3 last_cursor_pos;
public:
    static DebugInput *debugInput;

    ~DebugInput();

    void handle_movement();

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

    static void cursor_position_callback(GLFWwindow *window, double x_pos, double y_pos);

    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
};

