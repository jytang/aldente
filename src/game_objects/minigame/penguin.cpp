#include "penguin.h"
#include "asset_loader.h"
#include "events.h"

Penguin::Penguin(int id) : GameObject(id) {
    tag = "PENGUIN";

    if (id == ON_SERVER) {
        move_speed = rand() % 5 + 1;

        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_capsule;
        rigid.mass = 10;
        events::add_rigidbody_event(rigid);

        // Lock y-axis and z-axis
        rigidbody->setLinearFactor(btVector3(1, 0.0f, 0.0f));

        //Lock angular rotation
        rigidbody->setAngularFactor(0);

        // Set spawn position
        set_position(glm::vec3(5, 0, rand() % 3 - 6));
    }
}

void Penguin::s_update_this() {
    btTransform t;

    // Get the transform from Bullet and into 't'
    rigidbody->getMotionState()->getWorldTransform(t);
    btVector3 to_set = t.getOrigin();

    // If asserts fail, please inform Kavin
    assert(!std::isnan(to_set.getX()));
    assert(!std::isnan(to_set.getZ()));

    transform.set_position(glm::vec3((float)to_set.getX(), (float)to_set.getY(),
        (float)to_set.getZ()));
}

void Penguin::c_update_state(glm::mat4 mat, bool enab) {
    anim_player.update();
    GameObject::c_update_state(mat, enab);
}

void Penguin::reset_position() {
    set_position(glm::vec3(5, 0, rand() % 3 - 6));
}

void Penguin::setup_model() {
    attach_model(AssetLoader::get_model("slime_blue"));
    transform.set_scale({ 0.004f, 0.004f, 0.004f });
    initial_transform.set_scale(transform.get_scale());
}