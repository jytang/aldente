#include "mobile_trap.h"

MobileTrap::MobileTrap(int x, int z, int id) : Trap(x, z, id) {
    if (id == ON_SERVER) {
        //Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_box;
        rigid.is_ghost = true;
        rigid.position = { x, 0.0f, z };
        events::add_rigidbody_event(rigid);

        notify_on_collision = true;

        // Initial Direction
        direction = { 0, 0, -1 };
    }
}

void MobileTrap::s_update_this() {

}

void MobileTrap::c_update_state(float x, float z, float wx, float wz, bool enab) {

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

    // Checks if collided with a wall
    if (move_type == MoveType::WALL) {
        
    }
}

void MobileTrap::c_on_collision(GameObject *other) {
    anim_player.play_if_paused("trigger");

    Player* player = dynamic_cast<Player*>(other);
    assert(player);
    player->c_take_damage();
}