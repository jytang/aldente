#include "player.h"

#include "asset_loader.h"
#include "assert.h"
#include "events.h"

#define ANIMATE_DELTA 0.001f

Player::Player() : GameObject() {
    tag = "PLAYER";
    to_moveX = 0;
    to_moveZ = 0;
    move_speed = 2.0f;

    events::RigidBodyData rigid;
    rigid.object = this;
    rigid.shape = hit_capsule;
    rigid.mass = 1;
    events::add_rigidbody_event(rigid);

    // Notify on collision.
    notify_on_collision = true;

    // Lock y-axis
    rigidbody->setLinearFactor(btVector3(1, 0.0f, 1));

    //Lock angular rotation
    rigidbody->setAngularFactor(0);
}

Player::Player(int obj_id) : GameObject(obj_id) {
    tag = "PLAYER";
}

// Just calls do_movement for now, can have more
// functionality later.
void Player::s_update_this() {

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
}

void Player::prepare_movement(int inX, int inZ) {
    to_moveX = inX;
    to_moveZ = inZ;
}

void Player::c_update_state(float x, float z, float wx, float wz, bool enab) {
    anim_player.update();
    float dx = std::fabs(x - transform.get_position().x);
    float dz = std::fabs(z - transform.get_position().z);
    bool animate = dx > ANIMATE_DELTA || dz > ANIMATE_DELTA;

    if (!animate) {
        if (!anim_player.check_paused())
            anim_player.stop();
    }
    else {
        if (anim_player.check_paused())
            anim_player.play();
    }

    GameObject::c_update_state(x, z, wx, wz, enab);
}

void Player::do_movement() {
    // Should account for deltatime so movement is
    // framerate independent? Unsure how Bullet handles framerate.
    rigidbody->setActivationState(true);
    rigidbody->setLinearVelocity(btVector3(to_moveX * move_speed, 0, to_moveZ * move_speed));
    transform.look_at(glm::vec3(to_moveX * move_speed, 0, to_moveZ * move_speed));
    direction = glm::vec3(to_moveX * move_speed, 0, to_moveZ * move_speed);
}

void Player::interact() {
    // Asks physics for a raycast to check if the player
    // is facing a construct.
    if (direction.x != 0 || direction.z != 0)
        events::dungeon::player_request_raycast_event(
            transform.get_position(), direction,
            [&](GameObject *bt_hit) {
                Construct *construct = dynamic_cast<Construct*>(bt_hit);

                // Register interacts only on constructs for now. Send the game object ID 
                // of the interacted construct to the server to process.
                if (construct) {
                    events::dungeon::network_interact_event(construct->get_id());
                }
            });
}

void Player::stop_walk() {
    anim_player.stop();
}

void Player::start_walk() {
    anim_player.set_speed(3.0f);
    anim_player.set_anim("walk");
    anim_player.set_loop(true);
    anim_player.play();
}

// Server collision
void Player::s_on_collision(GameObject *other) {
    // TODO: actual game logic here...

    // Then notify clients that this collision happened.
    events::dungeon::network_collision_event(id);
}

// Graphical collision
void Player::c_on_collision() {
    transform.rotate(0, 0.1f, 0);
}

void Player::set_start_position(glm::vec3 pos) {
    start_pos = pos;
}

void Player::reset_position() {
    set_position(start_pos);
    to_moveX = 0;
    to_moveZ = 0;

    // Face south.
    direction = glm::vec3(0, 0, 1);
    transform.look_at(direction);
}

void Player::setup_player_model(std::string model_name) {
    Model *player_model = AssetLoader::get_model(model_name);
    player_model->set_shader(&ShaderManager::anim_unlit);
    attach_model(player_model);
    start_walk();

    // Sets scale. Need better way to do this later.
    if (model_name == "boy_two")
        transform.set_scale({ 0.4f, 0.4f, 0.4f });
    else if (model_name == "cat")
        transform.set_scale({ 0.004f, 0.004f, 0.004f });
}
