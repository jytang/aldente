#include "slime.h"
#include "asset_loader.h"

Slime::Slime(int x, int z, int id) : MobileTrap(x, z, id) {
    if (id == ON_SERVER) {
        //Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_box_small;
        rigid.mass = 1;
        rigid.is_ghost = false;
        rigid.position = { x, 0.0f, z };
        events::add_rigidbody_event(rigid);

        // Lock y-axis
        rigidbody->setLinearFactor(btVector3(1, 0.0f, 1));

        // Move Speed
        move_speed = 2.0f;

        // Time movement based
        move_type = TIME;
        setup_timer(3000); // changes direction every 2 seconds

        // Rotation Amount
        random_rotations_on = true;
    }
}

void Slime::setup_model() {
    Model *model = AssetLoader::get_model("slime");
    attach_model(model);
    transform.set_scale({ 0.004f, 0.004f, 0.004f });

    anim_player.set_anim("walk");
    anim_player.set_loop(true);
    anim_player.play();
}