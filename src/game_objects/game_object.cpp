#include "game_object.h"
#include "util/util_bt.h"

int GameObject::id_counter = 0;

GameObject::GameObject() {
    model = new Model();
    rigidbody = nullptr;
    id = id_counter++;
}

GameObject::~GameObject(){

    //TODO : Cleanup for rigid bodies and removing from game

    for (GameObject *obj : children)
        delete(obj);
}

// Adopt a game object
void GameObject::add_child(GameObject *obj) {
    children.push_back(obj);
}

// Disown a single child (Note, this does not delete the object, in case of reparenting)
void GameObject::remove_child(GameObject *obj) {
    // I'm pretty sure this line of code works. Not tested.
    children.erase(std::remove(children.begin(), children.end(), obj), children.end());
}

// Get rid of all the children (Note, this deletes the object.)
void GameObject::remove_all() {
    for (GameObject *obj : children)
        delete(obj);
    children.clear();
}

// Renders model and children's models in scene
void GameObject::draw(Shader *shader, SceneInfo &scene_info) {
    if (model) {
        connect_skel_to_model();
        model->draw(shader, scene_info, transform.get_world_mat());
    }

    for (GameObject *obj : children)
        obj->draw(shader, scene_info);
}

// Runs Game Object's update loop
void GameObject::update() {

    update_this();

    for (GameObject *obj : children)
        obj->update();
}

// Attaches bones from skeleton to model
// Allows for different game objects using the same model
// to be animated independently. This needs to happen before each
// one's draw call (i.e. cannot just be done in update)
void GameObject::connect_skel_to_model() {
    model->set_bones(&skel);
}


// Sets the material color for the entire game object's model
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
