#include "spikes.h"
#include "asset_loader.h"

/************SPIKES***************/

Spikes::Spikes(int x, int z, int id) : Trap(x, z, id) {
    tag = "SPIKES";

    if (id == ON_SERVER) {
        //Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_box;
        rigid.is_ghost = true;
        rigid.position = { x, 0.0f, z };
        events::add_rigidbody_event(rigid);
        notify_on_collision = true;
    }
}

void Spikes::s_on_collision(GameObject *other) {
    Player *player = dynamic_cast<Player*>(other);
    if (player) {
        if (player->s_take_damage()) {
            // Send signal to client that this player was hit
            events::dungeon::network_collision_event(other->get_id(), id);
        }
    }
}

void Spikes::c_on_collision(GameObject *other) {
    anim_player.play_if_paused("trigger");

    Player* player = dynamic_cast<Player*>(other);
    assert(player);
    player->c_take_damage();
}

void Spikes::setup_model() {
    attach_model(AssetLoader::get_model("spikes"));
    transform.set_scale({ 0.4f, 0.4f, 0.4f });
}