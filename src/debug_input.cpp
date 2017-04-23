#include "debug_input.h"

#include "aldente.h"
#include "physics.h"

bool DebugInput::keys[1024];
bool DebugInput::lmb_down;
bool DebugInput::rmb_down;
bool DebugInput::mouse_moved;
glm::vec3 DebugInput::last_cursor_pos;

// TODO : This class won't be used in the final game anyways.
const GLfloat BASE_CAM_SPEED = 0.1f;
const GLfloat EDGE_PAN_THRESH = 5.f;
const GLfloat EDGE_PAN_SPEED = 0.5f;

// FPS Tracking Variables
GLuint frame = 0;
double prev_ticks = glfwGetTime();
double move_prev_ticks = prev_ticks;

DebugInput *DebugInput::debugInput = new DebugInput();

DebugInput::DebugInput() {
    lmb_down = false;
    rmb_down = false;
    mouse_moved = false;
}

DebugInput::~DebugInput() {}

void DebugInput::handle_movement() {
    // FPS Control / Tracking
    frame++;
    double curr_time = glfwGetTime();
    if (curr_time - prev_ticks > 1.f) {
        /* std::cerr << "FPS: " << frame << std::endl; */
        frame = 0;
        prev_ticks = curr_time;
        return;
    }
    if (curr_time - move_prev_ticks > 1.f / 60.f)
        move_prev_ticks = curr_time;

    SceneCamera *camera = Aldente::aldente->get_camera();

    GLfloat cam_step = keys[GLFW_KEY_LEFT_SHIFT] ? 3 * BASE_CAM_SPEED : BASE_CAM_SPEED;
    glm::vec3 displacement(0.f);
    if (keys[GLFW_KEY_W])
        displacement += cam_step * camera->cam_front;
    if (keys[GLFW_KEY_S])
        displacement -= cam_step * camera->cam_front;
    if (keys[GLFW_KEY_A])
        displacement -= glm::normalize(glm::cross(camera->cam_front, camera->cam_up)) * cam_step;
    if (keys[GLFW_KEY_D])
        displacement += glm::normalize(glm::cross(camera->cam_front, camera->cam_up)) * cam_step;
    if (keys[GLFW_KEY_SPACE])
        displacement += cam_step * camera->cam_up;

    camera->cam_pos += displacement;
    camera->recalculate();
}

void DebugInput::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Check for a key press
    if (action == GLFW_PRESS) {
        keys[key] = true;
        switch (key) {
            // Check if escape was pressed
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_Q:
                Aldente::aldente->debug_shadows = !Aldente::aldente->debug_shadows;
                break;
            case GLFW_KEY_X:
                Aldente::aldente->shadows_on = !Aldente::aldente->shadows_on;
                break;
            default:
                break;
        }
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
}

void DebugInput::cursor_position_callback(GLFWwindow *window, double x_pos, double y_pos) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glm::vec3 current_cursor_pos(x_pos, y_pos, 0);

    Scene *scene = Aldente::aldente->get_scene();
    SceneCamera *camera = Aldente::aldente->get_camera();

    // First movement detected.
    if (!mouse_moved) {
        mouse_moved = true;
        last_cursor_pos = current_cursor_pos;
        return;
    }

    glm::vec3 cursor_delta = current_cursor_pos - last_cursor_pos;
    if (lmb_down && keys[GLFW_KEY_LEFT_CONTROL]) {
        int dir = cursor_delta.x > 0 ? 1 : -1;
        float rot_angle = dir * glm::length(cursor_delta) * 0.001f;
        scene->light_pos = glm::vec3(
                glm::rotate(glm::mat4(1.0f), rot_angle, glm::vec3(0.f, 0.f, 1.f)) * glm::vec4(scene->light_pos, 1.0f));
    } else if (!keys[GLFW_KEY_LEFT_CONTROL]) {
        // Look around.
        GLfloat xoffset = cursor_delta.x;
        GLfloat yoffset = cursor_delta.y;
        GLfloat sensitivity = 0.5;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        if (current_cursor_pos.x > width - EDGE_PAN_THRESH)
            xoffset = EDGE_PAN_SPEED;
        else if (current_cursor_pos.x < EDGE_PAN_THRESH)
            xoffset = -EDGE_PAN_SPEED;

        camera->yaw += xoffset;
        camera->pitch += yoffset;

        if (camera->pitch > 89.0f)
            camera->pitch = 89.0f;
        if (camera->pitch < -89.0f)
            camera->pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
        front.y = -sin(glm::radians(camera->pitch));
        front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
        camera->cam_front = glm::normalize(front);
        camera->recalculate();
    }

    Physics::physics->raycast_mouse(x_pos, y_pos, width, height);

    last_cursor_pos = current_cursor_pos;
}

void DebugInput::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    double x_pos, y_pos;
    glfwGetCursorPos(window, &x_pos, &y_pos);

    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            if (action == GLFW_PRESS) {
                lmb_down = true;
                last_cursor_pos = glm::vec3(x_pos, y_pos, 0.f);
            } else if (action == GLFW_RELEASE) {
                lmb_down = false;
            }
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            if (action == GLFW_PRESS) {
                rmb_down = true;
                last_cursor_pos = glm::vec3(x_pos, y_pos, 0.f);
            } else if (action == GLFW_RELEASE) {
                rmb_down = false;
            }
            break;
        default:
            break;
    }
}

void DebugInput::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    SceneCamera *camera = Aldente::aldente->get_camera();
    glm::vec3 trans_vec = (float) yoffset * glm::normalize(camera->cam_front);
    // Only y is relevant here, -1 is down, +1 is up
    camera->cam_pos = glm::vec3(glm::translate(glm::mat4(1.0f), trans_vec) * glm::vec4(camera->cam_pos, 1.0f));
    camera->recalculate();
}
