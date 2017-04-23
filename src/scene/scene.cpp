#include "scene.h"

Scene::Scene() {
    root = new SceneGroup(this);
    camera = new SceneCamera(this);
}

Scene::~Scene() {
    delete (root);
    delete (camera);
}

void Scene::draw() {
    root->draw();
}

void Scene::update() {
    root->update();
}

void Scene::pass(Shader *s) {
    // For passing a different shader than the mesh's shader and drawing,
    // Used mainly for shadow mapping passes
    root->pass(s);
}