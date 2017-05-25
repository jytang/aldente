#include "collision_trap.h"

CollisionTrap::CollisionTrap(int x, int z, int id) : Trap(x, z, id) {
    if (id == ON_SERVER) {
        // We want collisions
        notify_on_collision = true;
    }
}

void CollisionTrap::s_on_collision(GameObject *other) {
    // Change if collider with a player
    Player *player = dynamic_cast<Player*>(other);
    if (player) {
        if (player->s_take_damage()) {
            // Send signal to client that this player was hit
            events::dungeon::network_collision_event(player->get_id(), id);
        }
    }
}

void CollisionTrap::c_on_collision(GameObject *other) {
    // Plays the "trigger" animation, if it has one.
    play_trigger_animation();

    Player* player = dynamic_cast<Player*>(other);
    assert(player);
    player->c_take_damage();
}