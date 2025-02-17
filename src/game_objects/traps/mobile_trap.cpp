#include "mobile_trap.h"
#include "game_objects/tile.h"
#include "util/util_bt.h"
#include "timer.h"

MobileTrap::MobileTrap(int x, int z, int id) : CollisionTrap(x, z, id), cancel_timer([]() {}) {
}

// Moves forward
void MobileTrap::s_update_this() {

    if (move_type == MoveType::WALL)
        check_wall();
    else if (move_type == MoveType::AI)
        update_ai();

    handle_movement();

    sync_position();
}

// Moves object forward
void MobileTrap::handle_movement() {
    if (stop_moving) {
        rigidbody->setLinearVelocity(btVector3(0, 0, 0));
        rigidbody->setActivationState(false);
    }
    else {
        rigidbody->setActivationState(true);
        rigidbody->setLinearVelocity(util_bt::convert_vec3(transform.get_forward() * move_speed));
    }
}

// Rotates a certain amount based. Needed since traps can only move forward. 
void MobileTrap::change_direction() {

    if (random_rotations_on)
        rotation_amount = Util::random(10.0f, 350.0f);

    transform.rotate({ 0.0f, rotation_amount, 0.0f });
}

// If detects a wall in front, changes direction
void MobileTrap::check_wall() {
    events::dungeon::request_raycast_event(
        transform.get_position(), transform.get_forward(), 0.6f,
        [&](GameObject *bt_hit) {
        TreeTile *wall = dynamic_cast<TreeTile*>(bt_hit);
        if (wall) {
            change_direction();
        }
    });
}

// Changes direction every interval.
void MobileTrap::setup_timer(long long time_interval_ms) {
    // cancels any previous timer, if exists.
    cancel_timer();

    cancel_timer = Timer::get()->do_every(
        std::chrono::milliseconds(time_interval_ms),
        [&]() {

        change_direction();
    });
}

// Resets direction and position
void MobileTrap::s_reset() {
    reset_to_initial_transform();
}

// Resets rotations and position
void MobileTrap::c_reset() {
    reset_to_initial_transform();
}