#include "construct.h"
#include "asset_loader.h"

Construct::Construct(int x, int z) {
    transform.set_position(x, 0.0f, z);
}

Crate::Crate(int x, int z) : Construct(x, z) {
    model = AssetLoader::get_model("chest_good_scaled");
    transform.set_scale(0.75f, 0.75f, 0.75f);

    events::RigidBodyData rigid = {
        glm::vec3(x,0.5f,z), //position
        0, //mass
        hit_box, //btshape
        glm::vec3(0,0,0), //inertia
        this, //the gameobject
        true
    };
    events::add_rigidbody_event(rigid);
}

// Activated when a player presses A on it
void Crate::interact_trigger() {
    model = AssetLoader::get_model("veggie");
}
