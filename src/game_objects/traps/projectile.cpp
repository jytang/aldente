#include "projectile.h"
#include "util/util_bt.h"
#include "game_objects/player.h"
#include "game_objects/tile.h"
#include "events.h"

#include "util/util.h"
#include <iostream>

Projectile::Projectile(int id) : GameObject(id), cancel_timer([]() {}) {
    if (id == ON_SERVER) {
        notify_on_collision = true;
    }
}

// Enables and orients projectile to spawn at spawn_location and go forward.
void Projectile::fire(Transform spawn_location) {
    // Spawn object in front of trap
    glm::vec3 offset = ((spawn_location.get_forward() * 0.5f) + (spawn_location.get_up() * height_offset));
    transform.look_at(spawn_location.get_forward());
    set_position(spawn_location.get_position() + offset);

    setup_timer(); // Set-up Time-Out. 

    enable();
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

            // Timer may be needed if want animation
            disable();
        }
    }

    if (dynamic_cast<TreeTile*>(other)) {
        disable();
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
    cancel_timer();

    // Disable object if it times_out
    cancel_timer = Timer::get()->do_after(
        std::chrono::milliseconds(time_out_ms),
        [&]() {
        disable();
    });
}

// For properly attaching parent
void Projectile::set_parent(int p_id) {
    if(GameObject::game_objects[p_id])
        GameObject::game_objects[p_id]->add_child(this);
    set_parent_id(p_id);
}