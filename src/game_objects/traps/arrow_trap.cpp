#include "arrow_trap.h"
#include "arrow.h"
#include "asset_loader.h"

ArrowTrap::ArrowTrap(int x, int z, int id) : ProjectileTrap(x, z, id) {
    if (id == ON_SERVER) {
        // Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_box;
        rigid.position = { x, 0.0f, z };
        events::add_rigidbody_event(rigid);

        //Arrow *arrow = new Arrow();
        //attach_projectile(arrow);

        // Fires arrow every two seconds
        setup_timer(2000);
    }
}

void ArrowTrap::setup_model() {
    Model *model = AssetLoader::get_model("arrow_trap");
    attach_model(model);
    transform.set_scale({ 0.004f, 0.004f, 0.004f });
    initial_transform.set_scale(transform.get_scale());

    anim_player.set_anim("spin");
    anim_player.set_loop(true);
    anim_player.play();
}