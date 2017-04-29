#include "game_object.h"

GameObject::GameObject() {
	model = new Model();
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

void GameObject::set_color(glm::vec3 color) {
    for (Mesh* mesh : model->meshes) {
        mesh->material->diffuse = color;
    }
}
