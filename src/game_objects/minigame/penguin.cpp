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

        // Lock z-axis
        rigidbody->setLinearFactor(btVector3(1, 1, 0.0f));

        rigidbody->setActivationState(true);

        // Set spawn position
        //row = rand() % 3 - 6;
        set_position(glm::vec3(5, 1, rand() % 10 - 5));
        transform.set_rotation(glm::vec3(0, -90, 0));

        move_speed = -(rand() % 3 + 3);
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

    // Forever moving left
    btVector3 vel = rigidbody->getLinearVelocity();
    float to_move = min(move_speed, vel.getX());
    rigidbody->setLinearVelocity(btVector3(to_move, vel.getY(), 0));

    events::dungeon::request_raycast_event(
        transform.get_position(), transform.get_forward(), 0.6f,
        [&](GameObject *bt_hit) {

        // If there's another penguin in front of it, give it a push
        if (bt_hit && bt_hit->tag == "PENGUIN") {
            bt_hit->get_rigid()->applyForce(btVector3(-1000, 0, 0), btVector3(0, 0, 0));
        }
    });
}

void Penguin::c_update_state(glm::mat4 mat, bool enab) {
    anim_player.update();
    GameObject::c_update_state(mat, enab);
}

void Penguin::reset_position() {
    set_position(glm::vec3(5, 1, rand() % 10 - 5));
}

void Penguin::setup_model() {
    attach_model(AssetLoader::get_model("slime_blue"));
    transform.set_scale({ 0.006f, 0.006f, 0.006f });
    initial_transform.set_scale(transform.get_scale());

    anim_player.set_anim("walk");
    anim_player.set_loop(true);
    anim_player.play();
}