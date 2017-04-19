#include "scene.h"
#include "util/util.h"
#include "global.h"
const GLfloat PLAYER_HEIGHT = Global::PLAYER_HEIGHT;

Scene::Scene()
{
    root = new SceneGroup(this);
    camera = new SceneCamera(this);
}

Scene::~Scene()
{
    delete(root);
    delete(camera);
}

void Scene::render()
{
    root->draw(glm::mat4(1.f));
}

void Scene::pass(Shader * s)
{
    root->pass(glm::mat4(1.f), s);
}

void Scene::displace_cam(glm::vec3 displacement)
{
    const GLfloat   SIZE = get_size(); // Base on current scene size.
    const GLfloat   EDGE_LEEWAY = SIZE / 6.f;
    const GLfloat   MOVE_BOUNDS = SIZE + EDGE_LEEWAY;
    const GLfloat   EDGE_THRESH = SIZE / 30.f;

    glm::vec3 new_pos = camera->cam_pos + displacement;
    // Check horizontal bounds.
    if (new_pos.x < -MOVE_BOUNDS || new_pos.x > MOVE_BOUNDS || new_pos.z < -MOVE_BOUNDS || new_pos.z > MOVE_BOUNDS)
        return;

    // Smoother edge movement.
    if (new_pos.x < -MOVE_BOUNDS + EDGE_THRESH) {
        float diff = glm::abs(-MOVE_BOUNDS - new_pos.x);
        displacement *= diff / EDGE_THRESH;
    }
    else if (new_pos.x > MOVE_BOUNDS - EDGE_THRESH) {
        float diff = glm::abs(MOVE_BOUNDS - new_pos.x);
        displacement *= diff / EDGE_THRESH;
    }
    if (new_pos.z < -MOVE_BOUNDS + EDGE_THRESH) {
        float diff = glm::abs(-MOVE_BOUNDS - new_pos.z);
        displacement *= diff / EDGE_THRESH;
    }
    else if (new_pos.z > MOVE_BOUNDS - EDGE_THRESH) {
        float diff = glm::abs(MOVE_BOUNDS - new_pos.z);
        displacement *= diff / EDGE_THRESH;
    }
    new_pos = camera->cam_pos + displacement;

    // Fix height.
    float new_height = 0.f;
    new_pos.y = new_height + PLAYER_HEIGHT;

    camera->cam_pos = new_pos;
    camera->recalculate();
}

void Scene::update_frustum_corners(int width, int height, GLfloat far)
{
    GLfloat aspect_ratio = (float)width / (float)height;
    glm::vec3 cam_right = glm::cross(camera->cam_pos - (camera->cam_pos + camera->cam_front), camera->cam_up);

    // Hnear = 2 * tan(fov / 2) * nDis
    GLfloat near_height = 2 * glm::tan(glm::radians(45.f / 2)) * 0.1f;
    GLfloat near_width = near_height * aspect_ratio;

    GLfloat far_height = 2 * glm::tan(glm::radians(45.f / 2)) * far;
    GLfloat far_width = far_height * aspect_ratio;

    glm::vec3 near_center = camera->cam_pos + camera->cam_front * 0.1f;
    glm::vec3 far_center = camera->cam_pos + camera->cam_front * far;

    frustum_corners[0] = near_center + (camera->cam_up * (near_height / 2)) - (cam_right * (near_width / 2));
    frustum_corners[1] = near_center + (camera->cam_up * (near_height / 2)) + (cam_right * (near_width / 2));
    frustum_corners[2] = near_center - (camera->cam_up * (near_height / 2)) - (cam_right * (near_width / 2));
    frustum_corners[3] = near_center - (camera->cam_up * (near_height / 2)) + (cam_right * (near_width / 2));
    frustum_corners[4] = far_center + (camera->cam_up * (far_height / 2)) - (cam_right * (far_width / 2));
    frustum_corners[5] = far_center + (camera->cam_up * (far_height / 2)) + (cam_right * (far_width / 2));
    frustum_corners[6] = far_center - (camera->cam_up * (far_height / 2)) - (cam_right * (far_width / 2));
    frustum_corners[7] = far_center - (camera->cam_up * (far_height / 2)) + (cam_right * (far_width / 2));
}

/* ADAPTED FROM http://ruh.li/CameraViewFrustum.html */
void Scene::update_frustum_planes()
{
    glm::mat4 m = P*camera->V;
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
    for (int i = 0; i < 6; i++)
    {
        float length = glm::length(frustum_planes[i].normal);
        // THE FIX WAS TO NEGATE NORMALS SO THEY POINT OUTSIDE OF FRUSTUM
        frustum_planes[i].normal = -frustum_planes[i].normal / length;
        frustum_planes[i].d = frustum_planes[i].d / length;
    }
}

glm::mat4 Scene::frustum_ortho()
{
    const float FRINGE_X = 5.f * PLAYER_HEIGHT;
    const float FRINGE_Y = 5.f * PLAYER_HEIGHT;
    const float FRINGE_Z = 5.f * PLAYER_HEIGHT;

    static bool once = false;
    glm::mat4 light_view = glm::lookAt(light_pos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::vec3 corners_lightspace[8];

    for (int i = 0; i < 8; ++i)
    {
        corners_lightspace[i] = glm::vec3(light_view * glm::vec4(frustum_corners[i], 1.f));
    }

    glm::vec3 min = corners_lightspace[0];
    glm::vec3 max = corners_lightspace[0];
    for (int i = 0; i < 8; i++)
    {
        if (corners_lightspace[i].x > max.x)
            max.x= corners_lightspace[i].x;
        else if (corners_lightspace[i].x < min.x)
            min.x = corners_lightspace[i].x;
        if (corners_lightspace[i].y > max.y)
            max.y = corners_lightspace[i].y;
        else if (corners_lightspace[i].y < min.y)
            min.y = corners_lightspace[i].y;
        if (corners_lightspace[i].z > max.z)
            max.z = corners_lightspace[i].z;
        else if (corners_lightspace[i].z < min.z)
            min.z = corners_lightspace[i].z;
    }

    return glm::ortho(min.x-FRINGE_X, max.x+FRINGE_X, min.y-FRINGE_Y, max.y+FRINGE_Y, -max.z-FRINGE_Z, -min.z+FRINGE_Z);
}
