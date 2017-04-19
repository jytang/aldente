#include "scene_transform.h"
#include "shaders/shader.h"

SceneTransform::SceneTransform(Scene *scene, glm::mat4 m)
{
    this->scene = scene;
    transformation = m;
}

SceneTransform::SceneTransform() {}
SceneTransform::~SceneTransform() {}

void SceneTransform::draw(glm::mat4 m)
{
    glm::mat4 new_mat = m * transformation;
    SceneGroup::draw(new_mat);
}

void SceneTransform::update()
{

}

void SceneTransform::pass(glm::mat4 m, Shader *s)
{
    glm::mat4 new_mat = m * transformation;
    SceneGroup::pass(new_mat, s);
}
