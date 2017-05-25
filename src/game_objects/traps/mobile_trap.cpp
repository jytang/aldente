#include "mobile_trap.h"
#include "game_objects/tile.h"
#include "util/util_bt.h"
#include "timer.h"

MobileTrap::MobileTrap(int x, int z, int id) : CollisionTrap(x, z, id) {
    if (id == ON_SERVER) {
        // Initial Direction
        direction = { 0, 0, 1 };

        // Resets Trap Position on Start of Build Phase
        events::build::start_build_event.connect([&, x, z]() {
            set_position({ x, 0.0f, z });
        });
    }
}

// Moves forward
void MobileTrap::s_update_this() {

    if (move_type == MoveType::WALL)
        check_wall();
    else if (move_type == MoveType::AI)
        update_ai();

    handle_movement();

    // Update transform with bullet's
    btTransform t;
    rigidbody->getMotionState()->getWorldTransform(t);
    btVector3 to_set = t.getOrigin();
    transform.set_position(util_bt::convert_vec3(to_set));
}

// Moves object forward
void MobileTrap::handle_movement() {
    rigidbody->setActivationState(true);
    rigidbody->setLinearVelocity(util_bt::convert_vec3(direction * move_speed));
    transform.look_at(direction * move_speed);
}

// Rotates a certain amount based. Needed since traps can only move forward. 
void MobileTrap::change_direction() {

    if (random_rotations_on)
        rotation_amount = Util::random(10.0f, 350.0f);

    direction = glm::vec3(glm::rotate(glm::mat4(1.f), (float) glm::radians(rotation_amount), glm::vec3(0, 1, 0)) * glm::vec4(direction, 0));
}

// If detects a wall in front, changes direction
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

// Changes direction every interval.
void MobileTrap::setup_timer(long long time_interval_ms) {
    // cancels any previous timer, if exists.
    if (cancel_timer)
        cancel_timer();

    cancel_timer = Timer::get()->do_every(
        std::chrono::milliseconds(time_interval_ms),
        [&]() {

        change_direction();
    });
}