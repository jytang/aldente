#include "spikes.h"
#include "asset_loader.h"

/************SPIKES***************/

Spikes::Spikes(int x, int z, int id) : CollisionTrap(x, z, id) {
    tag = "SPIKES";

    if (id == ON_SERVER) {
        //Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_box;
        rigid.is_ghost = true;
        rigid.position = { x, 0.0f, z };
        events::add_rigidbody_event(rigid);
    }
}

void Spikes::play_trigger_animation() {
    anim_player.play_if_paused("trigger");
}

void Spikes::setup_model() {
    attach_model(AssetLoader::get_model("spikes"));
    transform.set_scale({ 0.5f, 2.0f, 0.5f });
}