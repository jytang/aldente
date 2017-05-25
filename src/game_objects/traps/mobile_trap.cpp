#include "mobile_trap.h"
#include "game_objects/tile.h"
#include "util/util_bt.h"

MobileTrap::MobileTrap(int x, int z, int id) : Trap(x, z, id) {
    if (id == ON_SERVER) {
        //Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_box;
        rigid.mass = 1;
        rigid.is_ghost = true; // Should they be ghosts? Not sure
        rigid.position = { x, 0.0f, z };
        events::add_rigidbody_event(rigid);

        notify_on_collision = true;

        // Initial Direction
        direction = { 0, 0, 1 };

        // Lock y-axis
        rigidbody->setLinearFactor(btVector3(1, 0.0f, 1));

        //Lock angular rotation
        rigidbody->setAngularFactor(0);

        // Default MoveSpeed
        move_speed = 1.0f;
    }
}

// Moves forward
void MobileTrap::s_update_this() {

    check_wall();

    handle_movement();

    // Update transform with bullet's
    btTransform t;
    rigidbody->getMotionState()->getWorldTransform(t);
    btVector3 to_set = t.getOrigin();
    transform.set_position(util_bt::convert_vec3(to_set));
}

void MobileTrap::s_on_collision(GameObject *other) {
    // Change if collider with a player
    Player *player = dynamic_cast<Player*>(other);
    if (player) {
        if (player->s_take_damage()) {
            // Send signal to client that this player was hit
            events::dungeon::network_collision_event(player->get_id(), id);
        }
    }
}

void MobileTrap::c_on_collision(GameObject *other) {
    anim_player.play_if_paused("trigger");

    Player* player = dynamic_cast<Player*>(other);
    assert(player);
    player->c_take_damage();
}

// Moves object forward
void MobileTrap::handle_movement() {
    rigidbody->setActivationState(true);
    rigidbody->setLinearVelocity(util_bt::convert_vec3(direction * move_speed));
    transform.look_at(direction * move_speed);
}

void MobileTrap::change_direction() {
    // Just turning 90 degrees for now TODO
    direction = glm::vec3(glm::rotate(glm::mat4(1.f), (float) glm::radians(90.0f), glm::vec3(0, 1, 0)) * glm::vec4(direction, 0));
}

// If detects a wall in front, turns the monster
void MobileTrap::check_wall() {
    events::dungeon::request_raycast_event(
        transform.get_position(), direction,
        [&](GameObject *bt_hit) {
        WallTile *wall = dynamic_cast<WallTile*>(bt_hit);
        if (wall) {
            change_direction();
        }
    });
}