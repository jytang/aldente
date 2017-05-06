#include "game_object.h"

int GameObject::id_counter = 0;

GameObject::GameObject() {
    model = new Model();
    rigidbody = nullptr;
    id = id_counter++;
}

// Renders model in scene
void GameObject::draw(Shader *shader, SceneInfo &scene_info) {
    if (model) {
        connect_skel_to_model();
        model->draw(shader, scene_info, transform.get_world_mat());
    }
}

// Updates Game Object Paramters
void GameObject::update() {
}

// Attaches bones from skeleton to model
// Allows for different game objects using the same model
// to be animated independently. This needs to happen before each
// one's draw call (i.e. cannot just be done in update)
void GameObject::connect_skel_to_model() {
    model->set_bones(&skel);
}

void GameObject::set_color(Color color) {
    for (Mesh* mesh : model->meshes) {
        mesh->material->diffuse = color;
    }
}
