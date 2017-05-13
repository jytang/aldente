#include "debug_input.h"

#include "util/config.h"
#include "game/game_state.h"

static const GLfloat BASE_CAM_SPEED = 0.1f;
static const GLfloat EDGE_PAN_THRESH = 5.f;
static const GLfloat EDGE_PAN_SPEED = 0.5f;

// TODO: this is disgusting
DebugInput::DebugInput(Window &window, SceneManager &scene_manager, Physics &p) :
        window(window),
        lmb_down(false), rmb_down(false),
        last_cursor_pos(0, 0, 0), scene_manager(scene_manager), physics(p) {

    std::fill(std::begin(keys), std::end(keys), false);

    // Call resize_callback once to set initial fov, etc.
    // TODO: This call should be placed elsewhere
    int w, h;
    std::tie(w, h) = window.get_size();
    events::WindowSizeData d = {&window, w, h};
    events::window_buffer_resize_event(d);

    // Set up callbacks
    events::window_key_event.connect([&](events::WindowKeyData d) {
        // Check for a key press
        if (d.action == GLFW_PRESS) {
            keys[d.key] = true;
            switch (d.key) {
                // Check if escape was pressed
                case GLFW_KEY_ESCAPE:
                    // Close the window. This causes the program to also terminate.
                    d.window->close();
                    break;
                case GLFW_KEY_Q:
                    events::debug::toggle_debug_shadows_event();
                    break;
                case GLFW_KEY_U:
                    events::debug::toggle_ui_event();
                    break;
                case GLFW_KEY_L:
                    events::debug::toggle_light_rotation_event();
                    break;
                case GLFW_KEY_V:
                    events::debug::toggle_debug_input_event();
                    break;
                case GLFW_KEY_0:
                    // FIXME(metakirby5)
                    break;
                case GLFW_KEY_1:
                    events::debug::client_set_phase_event(&GameState::build_phase);
                    break;
                case GLFW_KEY_2:
                    events::debug::client_set_phase_event(&GameState::dungeon_phase);
                    break;
                case GLFW_KEY_3:
                    // FIXME(metakirby5)
                    break;
                default:
                    break;
            }
        } else if (d.action == GLFW_RELEASE) {
            keys[d.key] = false;
        }

        if (debug_input_on)
            debug_movement_keys();

    });

    events::window_cursor_event.connect([&](events::WindowCursorData d) {
        int width, height;
        std::tie(width, height) = d.window->get_size();
        glm::vec3 current_cursor_pos(d.x_pos, d.y_pos, 0);

        if (debug_input_on)
            debug_movement_mouse(d);

        last_cursor_pos = current_cursor_pos;
    });
}

// TODO: change this to a "FrameEdge" event callback
void DebugInput::debug_movement_keys() {

    Camera *camera = scene_manager.get_camera();

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

    camera->displace_cam(displacement);
}

void DebugInput::debug_movement_mouse(events::WindowCursorData d) {

    int width, height;
    std::tie(width, height) = d.window->get_size();

    Camera *camera = scene_manager.get_camera();
    glm::vec3 current_cursor_pos(d.x_pos, d.y_pos, 0);

    glm::vec3 cursor_delta = current_cursor_pos - last_cursor_pos;

    // Look around.
    GLfloat xoffset = cursor_delta.x;
    GLfloat sensitivity = 0.5;
    GLfloat yoffset = cursor_delta.y;
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
    glm::vec3 left = glm::cross(glm::vec3(0, 1, 0), camera->cam_front);
    camera->cam_up = glm::cross(camera->cam_front, left);
    camera->recalculate();

}
