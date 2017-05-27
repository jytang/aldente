#include "projectile.h"
#include "util/util_bt.h"
#include "game_objects/player.h"
#include "events.h"

Projectile::Projectile(int id) : GameObject(id) {
    if (id == ON_SERVER) {
        notify_on_collision = true;
    }
}

// Enables and orients projectile to spawn at spawn_location and go forward.
void Projectile::fire(Transform spawn_location) {
    enable();

    transform.set_position(spawn_location.get_position());
    transform.set_rotation(spawn_location.get_rotation());
}

void Projectile::s_update_this() {
    handle_movement();
    sync_position();
}

void Projectile::s_on_collision(GameObject *other) {
    Player *player = dynamic_cast<Player*>(other);
    if (player) {
        if (player->s_take_damage()) {
            // Send signal to client that this player was hit
            events::dungeon::network_collision_event(player->get_id(), id);
        }
    }
}

void Projectile::c_on_collision(GameObject *other) {
    // Plays the "trigger" animation, if it has one.
    play_trigger_animation();

    Player* player = dynamic_cast<Player*>(other);
    assert(player); // Should only be called if hit player
    player->c_take_damage();
}

void Projectile::handle_movement() {
    rigidbody->setActivationState(true);
    rigidbody->setLinearVelocity(util_bt::convert_vec3(transform.get_forward() * speed));
}

void Projectile::setup_timer() {
    // cancels any previous timer, if exists.
    if (cancel_timer)
        cancel_timer();

    // Disable object if it times_out
    cancel_timer = Timer::get()->do_after(
        std::chrono::milliseconds(time_out_ms),
        [&]() {
        disable();
    });
}