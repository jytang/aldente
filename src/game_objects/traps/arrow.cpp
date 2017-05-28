#include "arrow.h"
#include "events.h"
#include "asset_loader.h"

Arrow::Arrow(int id) : Projectile(id) {
    projectile_type = ProjectileTypes::ARROW;

    if (id == ON_SERVER) {
        // Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_box;
        rigid.mass = 1;
        rigid.is_ghost = true;
        events::add_rigidbody_event(rigid);

        // Lock y-axis
        rigidbody->setLinearFactor(btVector3(1, 0.0f, 1));

        // Fast speed
        speed = 10.0f;

        // 5 Seconds time out
        time_out_ms = 5000;
    }
}

void Arrow::play_trigger_animation() {
    // This is where a cool animation/effects would play once the arrow hits...

    // IF WE HAD ONE
}

void Arrow::setup_model() {
    Model *model = AssetLoader::get_model("arrow");
    attach_model(model);
    transform.set_scale({ 0.01f, 0.01, 0.01f });
    initial_transform.set_scale(transform.get_scale());

    anim_player.set_anim("spin");
    anim_player.set_loop(true);
    anim_player.play();
}