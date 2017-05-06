#include "player.h"

#include "asset_loader.h"
#include "events.h"
#include "assert.h"

Player::Player() : GameObject() {
    to_moveX = 0;
    to_moveZ = 0;
    move_speed = 2.0f;
    direction = glm::vec3(0.0f);

    events::RigidBodyData rigid = {
        glm::vec3(0.0f, 0.0f, 0.0f), //position
        1, //mass
        hit_capsule, //btshape
        glm::vec3(0,0,0), //inertia
        this, //the gameobject
    };
    events::add_rigidbody_event(rigid);

    // Lock y-axis
    rigidbody->setLinearFactor(btVector3(1, 0.0f, 1));
    //Lock angular rotation
    rigidbody->setAngularFactor(0);

    setup_listeners();

    set_position({ 2.0f, 0.0f, 2.0f });
}

void Player::setup_listeners() {
    events::dungeon::player_move_event.connect([&](events::StickData d) {
        to_moveX = d.state.first;
        to_moveZ = d.state.second;
    });

    events::dungeon::player_interact_event.connect([&]() {
        interact();
    });
}

// Just calls do_movement for now, can have more
// functionality later.
void Player::update() {
    // Test code for playing animation for the boy
    anim_player.update();

    do_movement();

    btTransform t;

    // Get the transform from Bullet and into 't'
    rigidbody->getMotionState()->getWorldTransform(t);
    btVector3 to_set = t.getOrigin();

    // If asserts fail, please inform Kavin
    assert(!std::isnan(to_set.getX()));
    assert(!std::isnan(to_set.getZ()));

    transform.set_position(glm::vec3((float)to_set.getX(), (float)to_set.getY(),
        (float)to_set.getZ()));

    // Fires player position whenever player moves
    events::dungeon::player_position_updated_event(transform.get_position());
}

void Player::do_movement() {
    // Should account for deltatime so movement is
    // framerate independent? Unsure how Bullet handles framerate.
    rigidbody->setActivationState(true);
    rigidbody->setLinearVelocity(btVector3(to_moveX * move_speed, 0, to_moveZ * move_speed));
    transform.look_at(glm::vec3(to_moveX * move_speed, 0, to_moveZ * move_speed));
    direction = glm::vec3(to_moveX * move_speed, 0, to_moveZ * move_speed);
    if (to_moveX == 0 && to_moveZ == 0) {
        if (!anim_player.check_paused()) {
            anim_player.stop();
        }
    }
    else {
        if (anim_player.check_paused()) {
            anim_player.play();
        }
    }

}

void Player::interact() {
    // Asks physics for a raycast to check if the player
    // is facing a construct.
    if (direction.x != 0 || direction.z != 0)
        events::dungeon::player_request_raycast_event(
            transform.get_position(), direction,
            [&](GameObject *bt_hit) {
                Construct *construct = dynamic_cast<Construct*>(bt_hit);
                if (construct) {
                    construct->interact_trigger();
                }
            });
}

void Player::stop_walk() {
    anim_player.stop();
}

void Player::start_walk() {
    anim_player.set_speed(3.0f);
    anim_player.set_anim(&skel, "walk");
    anim_player.set_loop(true);
    anim_player.play();
}

void Player::on_collision(GameObject *other) {
    transform.set_scale(transform.get_scale() * 0.99f);
}
