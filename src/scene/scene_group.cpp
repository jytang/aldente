#include "scene_group.h"

SceneGroup::SceneGroup() {}

SceneGroup::SceneGroup(Scene *scene) {
    this->scene = scene;
}

SceneGroup::~SceneGroup() {
    for (auto it = children.begin(); it != children.end(); ++it)
        delete (*it);
}

void SceneGroup::add_child(SceneNode *node) {
    children.push_back(node);
}

void SceneGroup::remove_all() {
    for (auto it = children.begin(); it != children.end(); ++it)
        delete (*it);
    children.clear();
}

void SceneGroup::draw() {
    for (auto it = children.begin(); it != children.end(); ++it)
        (*it)->draw();
}

void SceneGroup::update() {
    for (auto it = children.begin(); it != children.end(); ++it)
        (*it)->update();
}

void SceneGroup::pass(Shader *s) {
    for (auto it = children.begin(); it != children.end(); ++it)
        (*it)->pass(s);
}
