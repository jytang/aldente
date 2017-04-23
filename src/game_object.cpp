#include "game_object.h"

GameObject::GameObject(Scene *scene) {
    this->scene = scene;
}


GameObject::~GameObject() {
}

// Attaches a model to the game object
void GameObject::attach_model(Model *m) {
    model = m;
}

// Renders model in scene
void GameObject::draw() {
    if (model)
        model->draw();
}

// Updates Game Object Paramters
void GameObject::update() {
    // Updates Model if Needed Here,
    // especially used if animation attached
    if (model)
        model->update();
}

// Used for passing seperate shaders, such as for shadows
void GameObject::pass(Shader *s) {
    if (model)
        model->pass(s);
}
