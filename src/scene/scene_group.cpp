#include "scene_group.h"
#include "shaders/shader.h"

SceneGroup::SceneGroup() {}

SceneGroup::SceneGroup(Scene *scene)
{
    this->scene = scene;
}

SceneGroup::~SceneGroup()
{
    for (auto it = children.begin(); it != children.end(); ++it)
        delete(*it);
}

void SceneGroup::add_child(SceneNode *node)
{
    children.push_back(node);
}

void SceneGroup::remove_all()
{
    for (auto it = children.begin(); it != children.end(); ++it)
        delete(*it);
    children.clear();
}

void SceneGroup::draw(glm::mat4 m)
{
    for (auto it = children.begin(); it != children.end(); ++it)
        (*it)->draw(m);
}

void SceneGroup::update()
{
    for (auto it = children.begin(); it != children.end(); ++it)
        (*it)->update();
}

void SceneGroup::pass(glm::mat4 m, Shader *s)
{
    for (auto it = children.begin(); it != children.end(); ++it)
        (*it)->pass(m, s);
}
