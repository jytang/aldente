#include "window.h"
#include "events.h"

bool Window::initted = false;
std::unordered_map<GLFWwindow *, Window *> Window::registry;

void Window::init() {
    // 4x antialiasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Platform-specific items
#if defined(__linux) || defined(__APPLE__)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

#ifdef __linux
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#else // Apple is dumb, requires hinting 3.2 to use 3.3+ features
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
#endif

    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

Window::Window(const std::string &name, bool show_cursor,
               int width_, int height_, bool fullscreen, GLFWmonitor *monitor) :
        width(width_), height(height_) {

    // Lazy one-time init
    if (!initted) {
        init();
        initted = true;
    }

    // Ensure we have dimensions
    assert(fullscreen || (width && height));

    // Create the GLFW window
    if (fullscreen) {
        monitor = glfwGetPrimaryMonitor();
        // Fullscreen
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        width = mode->width;
        height = mode->height;
    }
    gl_window = glfwCreateWindow(width, height, name.c_str(), monitor, nullptr);
    registry[gl_window] = this;
    glfwMakeContextCurrent(gl_window);
    glfwSwapInterval(1);

    if (!show_cursor) {
        glfwSetInputMode(gl_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    // Ensure own width and height is accurate to glfw
    glfwGetFramebufferSize(gl_window, &width, &height);
    events::window_buffer_resize_event.connect([&](events::WindowSizeData d) {
        if (this != d.window) return;
        width = d.width;
        height = d.height;
    });

    // Set up event dispatchers
    glfwSetFramebufferSizeCallback(gl_window, [](GLFWwindow *window, int w, int h) {
        glfwMakeContextCurrent(window);
        events::WindowSizeData d = {lookup(window), w, h};
        events::window_buffer_resize_event(d);
    });
    glfwSetKeyCallback(gl_window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        glfwMakeContextCurrent(window);
        events::WindowKeyData d = {lookup(window), key, scancode, action, mods};
        events::window_key_event(d);
    });
    glfwSetCursorPosCallback(gl_window, [](GLFWwindow *window, double x_pos, double y_pos) {
        glfwMakeContextCurrent(window);
        events::WindowCursorData d = {lookup(window), x_pos, y_pos};
        events::window_cursor_event(d);
    });
    glfwSetMouseButtonCallback(gl_window, [](GLFWwindow *window, int button, int action, int mods) {
        glfwMakeContextCurrent(window);
        events::WindowMouseButtonData d = {lookup(window), button, action, mods};
        events::window_mouse_button_event(d);
    });
    glfwSetScrollCallback(gl_window, [](GLFWwindow *window, double x_off, double y_off) {
        glfwMakeContextCurrent(window);
        events::WindowCursorData d = {lookup(window), x_off, y_off};
        events::window_cursor_event(d);
    });
}

Window::~Window() {
    glfwDestroyWindow(gl_window);
}

Window *Window::lookup(GLFWwindow * target) {
    auto found = registry.find(target);
    return found != registry.end() ? found->second : nullptr;
}

void Window::close() {
    glfwSetWindowShouldClose(gl_window, GL_TRUE);
}

int Window::should_close() const {
    return glfwWindowShouldClose(gl_window);
}

void Window::swap_buffers() {
    glfwSwapBuffers(gl_window);
}

void Window::clear() const {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

std::pair<int, int> Window::get_size() const {
    return {width, height};
}

std::pair<double, double> Window::get_cursor() const {
    double x_pos, y_pos;
    glfwGetCursorPos(gl_window, &x_pos, &y_pos);
    return {x_pos, y_pos};
}

void Window::broadcast_size() {
    events::WindowSizeData d = {this, width, height};
    events::window_buffer_resize_event(d);
}
