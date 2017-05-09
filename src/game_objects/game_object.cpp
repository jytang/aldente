#include "game_object.h"

int GameObject::id_counter = 0;

GameObject::GameObject() {
    model = new Model();
    rigidbody = nullptr;
    id = id_counter++;
}

// Attaches a model to the game object
void GameObject::attach_model(Model *m) {
    model = m;
}

// Renders model in scene
void GameObject::draw(Shader *shader, SceneInfo &scene_info) {
    if (model) model->draw(shader, scene_info, transform.get_world_mat());
}

// Updates Game Object Paramters
void GameObject::update() {
}

void GameObject::set_color(Color color) {
    for (Mesh* mesh : model->meshes) {
        mesh->material->diffuse = color;
    }
}