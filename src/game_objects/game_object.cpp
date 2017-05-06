#include "game_object.h"
#include "util/util_bt.h"

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

// Sets position of both object's transform and rigid body
// Use this intead of transform.set_position to make sure that rigid body
// is located in the same place as the object
void GameObject::set_position(glm::vec3 pos) {

    // Set for Transform
    transform.set_position(pos);

    // Set for Rigid Body
    btTransform initialTransform;

    if (rigidbody) {
        initialTransform.setOrigin(util_bt::convert_vec3(pos));
        initialTransform.setRotation(btQuaternion::getIdentity());
        rigidbody->setWorldTransform(initialTransform);
        rigidbody->getMotionState()->setWorldTransform(initialTransform);
    }
}
