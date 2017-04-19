#include "scene_camera.h"
#include "shaders/shader.h"

SceneCamera::SceneCamera(Scene *scene)
{
    this->scene = scene;
    reset();
}

SceneCamera::~SceneCamera() {}

void SceneCamera::draw(glm::mat4 m)
{
}

void SceneCamera::update()
{
}

void SceneCamera::recalculate()
{
    V = glm::lookAt(cam_pos, cam_pos + cam_front, cam_up);
}

void SceneCamera::reset()
{
    cam_pos = DEFAULT_CAM_POS;
    cam_front = DEFAULT_CAM_FRONT;
    cam_up = DEFAULT_CAM_UP;
    recalculate();
}

void SceneCamera::pass(glm::mat4 m, Shader *s)
{
}
