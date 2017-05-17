#include "construct.h"
#include "asset_loader.h"
#include "player.h"

Construct::Construct(int x, int z) : GameObject() {
    tag = "CONSTRUCT";
    transform.set_position(x, 0.0f, z);
}

Construct::Construct(int x, int z, int id) : GameObject(id) {
    tag = "CONSTRUCT";
    transform.set_position(x, 0.0f, z);
}

Chest::Chest(int x, int z) : Construct(x, z) {
    transform.set_scale(0.6f, 0.6f, 0.6f);
    events::RigidBodyData rigid = {
        glm::vec3(x,0.5f,z), //position
        0, //mass
        hit_box, //btshape
        glm::vec3(0,0,0), //inertia
        this, //the gameobject
        false, // is a ghost object
    };
    events::add_rigidbody_event(rigid);
}

Chest::Chest(int x, int z, int id) : Construct(x, z, id) {
    transform.set_scale(0.6f, 0.6f, 0.6f);
}

void Chest::update_this() {
    anim_player.update();
}

// Activated when a player presses A on it
void Chest::interact_trigger() {
    anim_player.set_anim("open");
    anim_player.play();
}

void Chest::setup_model() {
    attach_model(AssetLoader::get_model("chest_good"));
    transform.set_scale({ 0.005f, 0.005f, 0.005f });
}

Goal::Goal(int x, int z) : Construct(x, z) {
    transform.set_scale(0.006f, 0.006f, 0.006f);
    tag = "GOAL";

    events::RigidBodyData rigid = {
        glm::vec3(x,0.5f,z), //position
        0, //mass
        hit_box, //btshape
        glm::vec3(0,0,0), //inertia
        this, //the gameobject
        true, // is a ghost object
    };
    events::add_rigidbody_event(rigid);

    notify_on_collision = true;
}

Goal::Goal(int x, int z, int id) : Construct(x, z, id) {
    transform.set_scale(0.006f, 0.006f, 0.006f);
    tag = "GOAL";
}

void Goal::setup_model() {
    attach_model(AssetLoader::get_model("warp"));
    model->set_shader(&ShaderManager::anim_unlit);
    anim_player.set_speed(1.0f);
    anim_player.set_anim("spin");
    anim_player.set_loop(false);
}

void Goal::on_collision(GameObject *other) {
    if (dynamic_cast<Player*>(other))
        events::dungeon::network_collision_event(id);
}

void Goal::on_collision_graphical() {
    if (anim_player.check_paused()) {
        anim_player.play();
    }
}

void Goal::update_this() {
    anim_player.update();
}
