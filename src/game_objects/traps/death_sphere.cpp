#include "death_sphere.h"
#include "events.h"
#include "asset_loader.h"

DeathSphere::DeathSphere(int id) : Projectile(id) {
    projectile_type = ProjectileTypes::SPHERE;

    if (id == ON_SERVER) {
        // Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_sphere;
        rigid.mass = 1;
        rigid.is_ghost = true;
        collision_group = COLLISION_TRAPS;
        events::add_rigidbody_event(rigid);

        // Lock y-axis
        rigidbody->setLinearFactor(btVector3(1, 0.0f, 1));

        // Speed
        speed = 4.0f;

        // Adjust height so that it rests on top of trap
        height_offset = 0.2f;

        // 5 Seconds time out
        time_out_ms = 5000;
    }
}

void DeathSphere::play_trigger_animation() {
}

void DeathSphere::setup_model() {
    Model *model = AssetLoader::get_model("austin");
    attach_model(model);
    set_scale({ 0.0025f, 0.0025f, 0.0025f });

    anim_player.set_anim("spin", 5.0f, true);
    anim_player.play();

    model->set_color({ 30.0f, 0.0f, 30.0f, false });
}