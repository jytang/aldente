#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "model/plane.h"

class SceneCamera {
private:
    glm::vec3 default_pos;
    glm::vec3 default_front;
    glm::vec3 default_up;

    bool follow_player;
public:
    glm::vec3 cam_pos;
    glm::vec3 cam_front;
    glm::vec3 cam_up;
    glm::mat4 P;
    glm::mat4 V;
    GLfloat pitch;
    GLfloat yaw = -90.f;

    bool disable_movement;

    Plane frustum_planes[6];
    glm::vec3 frustum_corners[8];

    SceneCamera(glm::vec3 default_pos = glm::vec3(0.f, 0.f, 20.f),
                glm::vec3 default_front = glm::vec3(0.f, 0.f, -1.f),
                glm::vec3 default_up = glm::vec3(0.f, 1.f, 0.f));

    void setup_listeners();

    void recalculate();

    void reset();

    void update_frustum_corners(int width, int height, GLfloat far_dist);

    void update_frustum_planes();

    void displace_cam(glm::vec3 displacement);

    glm::mat4 frustum_ortho(glm::vec3 light_pos);
};

