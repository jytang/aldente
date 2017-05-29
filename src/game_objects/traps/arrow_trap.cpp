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
        setup_timer(1000);
        activation_type = TIME;
    }
}

void ArrowTrap::setup_model() {
    Model *model = AssetLoader::get_model("arrow_trap");
    attach_model(model);
    transform.set_scale({ 0.0095f, 0.0095f, 0.0095f });
    initial_transform.set_scale(transform.get_scale());

    anim_player.set_anim("shoot");
    anim_player.set_loop(false);
    anim_player.set_speed(5.0f);
}

void ArrowTrap::play_trigger_animation() {
    anim_player.play();
}