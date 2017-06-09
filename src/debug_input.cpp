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
                case GLFW_KEY_TAB:
                    events::ui::toggle_leaderboard();
                    break;
                case GLFW_KEY_Q:
                    events::debug::toggle_debug_shadows_event();
                    break;
                case GLFW_KEY_L:
                    events::debug::toggle_light_rotation_event();
                    break;
                case GLFW_KEY_V:
                    events::debug::toggle_debug_input_event();
                    break;
                case GLFW_KEY_B:
                    events::debug::toggle_bt_debug_drawer_event();
                    events::debug::toggle_ui_text_box_background_event();
                    break;
                case GLFW_KEY_F:
                    events::debug::toggle_fps_event();
                    break;
                case GLFW_KEY_M:
                    events::toggle_mute_event();
                    break;
                case GLFW_KEY_F1:
                    events::ui::transition_wipe(1.f, "TO BE CONTINUED", [](){});
                    break;
                case GLFW_KEY_F2:
                    events::ui::transition_fade(1.f, "TO BE CONTINUED", [](){});
                    break;
                case GLFW_KEY_F3:
                    events::ui::scoreboard_sequence(
                            {{"pig", 123, 500},
                             {"lizar", 456, 7},
                             {"cat", 999, 5},
                             {"tomatoe", 1, 66666}
                            });
                    break;
                case GLFW_KEY_F4:
                    events::ui::disable_scoreboard();
                    break;
                case GLFW_KEY_F5:
                    events::debug::decrease_hdr_exposure_event();
                    break;
                case GLFW_KEY_F6:
                    events::debug::increase_hdr_exposure_event();
                    break;
                case GLFW_KEY_F7:
                    events::debug::toggle_hdr_event();
                    break;
                case GLFW_KEY_F8:
                    events::debug::toggle_bloom_event();
                    break;
                case GLFW_KEY_F9:
                    events::ui::enable_main_menu();
                    break;
                case GLFW_KEY_F10:
                    events::ui::disable_main_menu();
                    break;
                case GLFW_KEY_0:
                    events::debug::client_set_phase_event(&GameState::menu_phase);
                    break;
                case GLFW_KEY_1:
                    events::debug::client_set_phase_event(&GameState::build_phase);
                    break;
                case GLFW_KEY_2:
                    events::debug::client_set_phase_event(&GameState::dungeon_phase);
                    break;
                case GLFW_KEY_3:
                    events::debug::client_set_phase_event(&GameState::minigame_phase);
                    break;
                case GLFW_KEY_4:
                    // Camera animation debug. Feel free to remove if needed
                    events::camera_anim_position_event(glm::vec3(1, 0.5f, 0), 2000, [](){});
                    events::camera_anim_rotate_event(glm::vec3(0, 0, 1), 720, 2000, [](){});
                    break;
                case GLFW_KEY_5:
                    events::debug::print_camera_event();
                    break;
                // Ethan's testing stuff
                case GLFW_KEY_Z:
                    events::ui::show_countdown({"3", "2", "1", "GO"}, Color::BLACK, []() {
                        std::cerr << "counted\n";
                    });
                    break;
                default:
                    break;
            }
        } else if (d.action == GLFW_RELEASE) {
            keys[d.key] = false;
            switch (d.key) {
                default:
                    break;
            }
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

    events::debug::toggle_debug_input_event.connect([&]() {
        debug_input_on = !debug_input_on;
        scene_manager.get_camera()->disable_movement = debug_input_on;
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
    if (keys[GLFW_KEY_LEFT_CONTROL])
        return;
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
