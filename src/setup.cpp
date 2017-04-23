#include "setup.h"
#include "shaders/shader_manager.h"
#include "window.h"
#include "aldente.h"
#include "debug_input.h"

#include "events/input.h"

namespace events = kuuhaku::events;

void Setup::setup_shaders() {
    // Load shaders via a shader manager.
    ShaderManager::create_shader_program("basic");
    ShaderManager::create_shader_program("skybox");
    ShaderManager::create_shader_program("shadow");
    ShaderManager::create_shader_program("debug_shadow");
    ShaderManager::set_default("basic");
}

void Setup::setup_callbacks() {
    GLFWwindow *window = Aldente::aldente->get_window();
    glfwSetErrorCallback(Window::error_callback);
    glfwSetKeyCallback(window, DebugInput::key_callback);
    glfwSetCursorPosCallback(window, DebugInput::cursor_position_callback);
    glfwSetMouseButtonCallback(window, DebugInput::mouse_button_callback);
    glfwSetScrollCallback(window, DebugInput::scroll_callback);
    glfwSetFramebufferSizeCallback(window, Window::resize_callback);
    events::joystick_event.connect([](events::JoystickData &d) {
        fprintf(stderr,
                "JoystickEvent:\n"
                        "  id: %d\n"
                        "  is_button: %d\n"
                        "  input: %d\n"
                        "  state: %d\n",
                d.id, d.is_button, d.input, d.state);
    });
}

void Setup::setup_opengl() {
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        glfwTerminate();
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}
