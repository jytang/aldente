#include "arrow_trap.h"
#include "asset_loader.h"

ArrowTrap::ArrowTrap(int x, int z, int id) : ProjectileTrap(x, z, id) {
    if (id == ON_SERVER) {
        // Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_box;
        rigid.position = { x, 0.0f, z };
        events::add_rigidbody_event(rigid);

        fill_projectile_pool(ProjectileTypes::ARROW);

        //activation_type = RAYCAST;
        //raycast_range = 10.f;

        // Fires arrow every ...
        setup_timer(500);
        activation_type = TIME;
    }

    transform.rotate(0, 90.0f, 0);
}

void ArrowTrap::setup_model() {
    Model *model = AssetLoader::get_model("arrow_trap");
    attach_model(model);
    transform.set_scale({ 0.008f, 0.008f, 0.008f });
    initial_transform.set_scale(transform.get_scale());

    anim_player.set_anim("spin");
    anim_player.set_loop(true);
    anim_player.play();
}