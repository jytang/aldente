#include "slow_trap.h"
#include "asset_loader.h"

SlowTrap::SlowTrap(int x, int z, int id) : Trap(x, z, id) {
    tag = "SLOW_TRAP";

    if (id == ON_SERVER) {
        // Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_box;
        rigid.is_ghost = true;
        rigid.position = { x, 0.0f, z };
        collision_group = COLLISION_STRUCTS;
        events::add_rigidbody_event(rigid);

        notify_on_collision = true;
    }
}

void SlowTrap::s_on_collision(GameObject *other) {
    Player *player = dynamic_cast<Player*>(other);
    if (player) {
        player->s_slow()

        // Send signal to client that this player was hit
        events::dungeon::network_collision_event(player->get_id(), id);
    }
}

void SlowTrap::c_on_collision(GameObject *other) {
    Player* player = dynamic_cast<Player*>(other);
    if (player)
        player->c_slow();
}

void SlowTrap::setup_model() {
    attach_model(AssetLoader::get_model("water"));
    set_scale({ 0.004f, 0.004f, 0.004 });
}