#pragma once

#define GLFW_INCLUDE_GLEXT

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

/*
	Handles GLFW calls for creating and resizing the window.
	Currently assumes there to only be one window. 
*/
class Window {
private:
    Window();

public:
    static Window *window;

    ~Window();

    int width, height; // Width and Height of last updated window

    GLFWwindow *create_window();

    static void error_callback(int error, const char *description);

    static void resize_callback(GLFWwindow *window, int width, int height);

    void poll_events();

    int should_close(GLFWwindow *window);

    void swap_buffers(GLFWwindow *window);

    void clear_window();

    void update_size(GLFWwindow *window);

    void destroy(GLFWwindow *window);
};