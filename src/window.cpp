#include "window.h"
#include "aldente.h"
#include "util/config.h"

const bool FULLSCREEN = false;

Window *Window::window = new Window();

Window::Window() {}

Window::~Window() {}

GLFWwindow *Window::create_window() {
    // Get window width and height from config
    Config::config->get_value(Config::str_screen_width, width);
    Config::config->get_value(Config::str_screen_height, height);
    std::string game_name;
    Config::config->get_value(Config::str_game_name, game_name);
    const char *window_title = game_name.c_str();

    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return NULL;
    }

    // 4x antialiasing
    glfwWindowHint(GLFW_SAMPLES, 4);

#if defined(__linux) || defined(__APPLE__)
    // Ensure minimum OpenGL version is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

#ifdef __linux
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#else // Apple is dumb
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
#endif

    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create the GLFW window
    GLFWmonitor *monitor = NULL;
    if (FULLSCREEN) {
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        width = mode->width;
        height = mode->height;
    }
    GLFWwindow *window = glfwCreateWindow(width, height, window_title, monitor, NULL);

    // Check if the window could not be created
    if (!window) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window
    glfwMakeContextCurrent(window);

    // Set swap interval to 1
    glfwSwapInterval(1);

    // Get the width and height of the framebuffer to properly resize the window
    glfwGetFramebufferSize(window, &width, &height);


    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // Don't show cursor

    return window;
}

void Window::error_callback(int error, const char *description) {
    fputs(description, stderr);
}

void Window::resize_callback(GLFWwindow *window, int width, int height) {
    // Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
    glViewport(0, 0, width, height);

    float far_plane, fov;
    Config::config->get_value(Config::str_far_plane, far_plane);
    Config::config->get_value(Config::str_fov, fov);

    if (height > 0) {
        for (Scene *s : Aldente::aldente->get_scenes())
            s->camera->P = glm::perspective(fov, (float) width / (float) height, 0.1f, far_plane);
    }
}

void Window::poll_events() {
    glfwPollEvents();
}

int Window::should_close(GLFWwindow *window) {
    return glfwWindowShouldClose(window);
}

void Window::swap_buffers(GLFWwindow *window) {
    glfwSwapBuffers(window);
}

void Window::clear_window() {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::update_size(GLFWwindow *window) {
    glfwGetFramebufferSize(window, &width, &height);
    resize_callback(window, width, height);
}

void Window::destroy(GLFWwindow *window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}
