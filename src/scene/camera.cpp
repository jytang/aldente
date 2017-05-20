#include "camera.h"
#include "scene/scene.h"
#include "events.h"

const float PAN_SPEED = 0.1f;

Camera::Camera(glm::vec3 default_pos,
                         glm::vec3 default_front,
                         glm::vec3 default_up)
    : default_pos(default_pos), default_front(default_front),
      default_up(default_up) {
    reset();

    setup_listeners();
}

void Camera::setup_listeners() {

    events::build::start_build_event.connect([&]() {
        follow_player = false;
        follow_hover = true;
        cam_pos = glm::vec3(0, 6.0f, 5.0f);
        cam_front = glm::normalize(-cam_pos);
        glm::vec3 left = glm::cross(glm::vec3(0, 1, 0), cam_front);
        cam_up = glm::cross(cam_front, left);
        recalculate();
    });

    events::build::end_build_event.connect([&]() {
        follow_player = true;
        follow_hover = false;
    });

    // Commented this out in case we still wanted it later for debugging
    /*events::build::pan_camera_event.connect([&](std::pair<int, int> state) {
        if (disable_movement)
            return;

        // TODO: Currently this causes the camera to move discreetly in one
        // direction, but we want it to move smoothly. This will require an update function though.
        if (!follow_player) {
            displace_cam(glm::vec3(state.first, 0, state.second) * PAN_SPEED);
        }
    });*/

    events::build::hover_position_updated_event.connect([&](int x, int z) {
        if (disable_movement)
            return;

        if (follow_hover) {
            glm::vec3 pos = glm::vec3(x, 0, z);
            cam_pos = pos + glm::vec3(0, 6.0f, 6.0f);
            cam_front = glm::normalize(pos - cam_pos);
            glm::vec3 left = glm::cross(glm::vec3(0, 1, 0), cam_front);
            cam_up = glm::cross(cam_front, left);
            recalcuate();
        }
    });

    events::dungeon::player_position_updated_event.connect([&](glm::vec3 pos) {
        if (disable_movement)
            return;

        if (follow_player) {
            cam_pos = pos + glm::vec3(0, 6.0f, 6.0f);
            cam_front = glm::normalize(pos - cam_pos);
            glm::vec3 left = glm::cross(glm::vec3(0, 1, 0), cam_front);
            cam_up = glm::cross(cam_front, left);
            recalculate();
        }
    });
}

void Camera::recalculate() {
    V = glm::lookAt(cam_pos, cam_pos + cam_front, cam_up);
}

void Camera::reset() {
    cam_pos = default_pos;
    cam_front = default_front;
    cam_up = default_up;
    recalculate();
}

void Camera::displace_cam(glm::vec3 displacement) {
    cam_pos = cam_pos + displacement;
    recalculate();
}

// Called during update loop
void Camera::update() {

}

// Recalculate corners of frustum in world space.
// TODO: parameterize fov and near distance.
void Camera::update_frustum_corners(int width, int height, GLfloat far_dist) {
    GLfloat aspect_ratio = (float) width / (float) height;
    glm::vec3 cam_right = glm::cross(cam_pos - (cam_pos + cam_front), cam_up);

    // Hnear = 2 * tan(fov / 2) * nDis
    GLfloat near_height = 2 * glm::tan(glm::radians(45.f / 2)) * 0.1f;
    GLfloat near_width = near_height * aspect_ratio;

    GLfloat far_height = 2 * glm::tan(glm::radians(45.f / 2)) * far_dist;
    GLfloat far_width = far_height * aspect_ratio;

    glm::vec3 near_center = cam_pos + cam_front * 0.1f;
    glm::vec3 far_center = cam_pos + cam_front * far_dist;

    frustum_corners[0] = near_center + (cam_up * (near_height / 2)) - (cam_right * (near_width / 2));
    frustum_corners[1] = near_center + (cam_up * (near_height / 2)) + (cam_right * (near_width / 2));
    frustum_corners[2] = near_center - (cam_up * (near_height / 2)) - (cam_right * (near_width / 2));
    frustum_corners[3] = near_center - (cam_up * (near_height / 2)) + (cam_right * (near_width / 2));
    frustum_corners[4] = far_center + (cam_up * (far_height / 2)) - (cam_right * (far_width / 2));
    frustum_corners[5] = far_center + (cam_up * (far_height / 2)) + (cam_right * (far_width / 2));
    frustum_corners[6] = far_center - (cam_up * (far_height / 2)) - (cam_right * (far_width / 2));
    frustum_corners[7] = far_center - (cam_up * (far_height / 2)) + (cam_right * (far_width / 2));
}

glm::mat4 Camera::frustum_ortho(glm::vec3 light_pos) {
    // May need to do redo values below
    const float FRINGE_X = 0; // 5.f * 1.7f;
    const float FRINGE_Y = 0; // 5.f * 1.7f;
    const float FRINGE_Z = 0; // 5.f * 1.7f;

    // Transform camera frustum into coordinate system of light.
    glm::mat4 light_view = glm::lookAt(light_pos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::vec3 corners_lightspace[8];
    for (int i = 0; i < 8; ++i)
        corners_lightspace[i] = glm::vec3(light_view * glm::vec4(frustum_corners[i], 1.f));

    // Make AABB with some FRINGE padding.
    glm::vec3 min = corners_lightspace[0];
    glm::vec3 max = corners_lightspace[0];
    for (int i = 0; i < 8; i++) {
        max.x = glm::max(max.x, corners_lightspace[i].x);
        max.y = glm::max(max.y, corners_lightspace[i].y);
        max.z = glm::max(max.z, corners_lightspace[i].z);
        min.x = glm::min(min.x, corners_lightspace[i].x);
        min.y = glm::min(min.y, corners_lightspace[i].y);
        min.z = glm::min(min.z, corners_lightspace[i].z);
    }

    return glm::ortho(min.x - FRINGE_X, max.x + FRINGE_X,
                      min.y - FRINGE_Y, max.y + FRINGE_Y,
                      -max.z - FRINGE_Z, -min.z + FRINGE_Z);
}


/* ADAPTED FROM http://ruh.li/CameraViewFrustum.html */
// Currently unused, can use for view frustum culling
void Camera::update_frustum_planes() {
    glm::mat4 m = P * V;
    frustum_planes[0].normal.x = m[0][3] + m[0][0];
    frustum_planes[0].normal.y = m[1][3] + m[1][0];
    frustum_planes[0].normal.z = m[2][3] + m[2][0];
    frustum_planes[0].d = m[3][3] + m[3][0];

    // right
    frustum_planes[1].normal.x = m[0][3] - m[0][0];
    frustum_planes[1].normal.y = m[1][3] - m[1][0];
    frustum_planes[1].normal.z = m[2][3] - m[2][0];
    frustum_planes[1].d = m[3][3] - m[3][0];

    // bottom
    frustum_planes[2].normal.x = m[0][3] + m[0][1];
    frustum_planes[2].normal.y = m[1][3] + m[1][1];
    frustum_planes[2].normal.z = m[2][3] + m[2][1];
    frustum_planes[2].d = m[3][3] + m[3][1];

    // top
    frustum_planes[3].normal.x = m[0][3] - m[0][1];
    frustum_planes[3].normal.y = m[1][3] - m[1][1];
    frustum_planes[3].normal.z = m[2][3] - m[2][1];
    frustum_planes[3].d = m[3][3] - m[3][1];

    // near
    frustum_planes[4].normal.x = m[0][3] + m[0][2];
    frustum_planes[4].normal.y = m[1][3] + m[1][2];
    frustum_planes[4].normal.z = m[2][3] + m[2][2];
    frustum_planes[4].d = m[3][3] + m[3][2];

    // far
    frustum_planes[5].normal.x = m[0][3] - m[0][2];
    frustum_planes[5].normal.y = m[1][3] - m[1][2];
    frustum_planes[5].normal.z = m[2][3] - m[2][2];
    frustum_planes[5].d = m[3][3] - m[3][2];

    // normalize
    for (int i = 0; i < 6; i++) {
        float length = glm::length(frustum_planes[i].normal);
        // THE FIX WAS TO NEGATE NORMALS SO THEY POINT OUTSIDE OF FRUSTUM
        frustum_planes[i].normal = -frustum_planes[i].normal / length;
        frustum_planes[i].d = frustum_planes[i].d / length;
    }
}
