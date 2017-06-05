#pragma once

#define GLFW_INCLUDE_GLEXT

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <unordered_map>
#include <mutex>

/*
    High-level GLFWwindow abstraction which dispatches lambda-friendly events.
*/
class Window {
private:
    GLFWwindow *gl_window;
    int width, height; // Required for clearing the window.

    // One-time init
    static bool initted;
    static void init();

    // A static registry of windows is required to statically obtain the Window pointer corresponding to the GLFWwindow
    // we get from a GLFW callback.
    static std::unordered_map<GLFWwindow *, Window *> registry;
    static Window *lookup(GLFWwindow * target);

public:
    Window(const std::string &name, bool show_cursor, int width, int height,
           bool fullscreen = false, GLFWmonitor *monitor = nullptr);
    ~Window();

    void close();
    int should_close() const;
    void swap_buffers();
    void clear() const;
    std::pair<int, int> get_size() const;
    std::pair<double, double> get_cursor() const;
    void broadcast_size();
};
