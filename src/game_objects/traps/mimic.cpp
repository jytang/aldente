#include "mimic.h"
#include "asset_loader.h"

#define ANIMATE_DELTA 0.001f // Amount needed to play moving animation

Mimic::Mimic(int x, int z, int id) : Mimic(x, z, id) {
    if (id == ON_SERVER) {
        // Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_box;
        rigid.mass = 1;
        rigid.is_ghost = false;
        rigid.position = { x, 0.0f, z };
        events::add_rigidbody_event(rigid);

        // Lock y-axis
        rigidbody->setLinearFactor(btVector3(1, 0.0f, 1));

        move_speed = 2.0f;
        move_type = AI;
    }
}

// Play moving animation if it moves past a minimum amount for delta
void Mimic::c_update_state(glm::mat4 mat, bool enab) {
    // Find difference in x and z positions for animating.
    float dx = std::fabs(mat[3][0] - transform.get_position().x);
    float dz = std::fabs(mat[3][2] - transform.get_position().z);
    bool animate = dx > ANIMATE_DELTA || dz > ANIMATE_DELTA;

    if (!animate) {
        if (!anim_player.check_paused()) {
            anim_player.stop();
        }
    }
    else {
        if (anim_player.check_paused()) {
            anim_player.play();
        }
    }

    GameObject::c_update_state(mat, enab);
}

// Handles logic for whether the AI is still in range of the target
void Mimic::update_ai() {
    if (curr_target) {
        change_direction();
    }
}

// Looks at current target if available
void Mimic::change_direction() {
    if (curr_target)
        transform.look_at(curr_target->transform.get_position() - transform.get_position());
}

// Check if other is a player, than targets them to follow them
void Mimic::s_interact_trigger(GameObject *other) {
    Player *player = dynamic_cast<Player*>(other);
    if (player && !curr_target) {
        curr_target = player;
    }
}

void Mimic::setup_model() {
    Model *model = AssetLoader::get_model("chest_bad");
    attach_model(model);
    transform.set_scale({ 0.004f, 0.004f, 0.004f });
    initial_transform.set_scale(transform.get_scale());

    anim_player.set_anim("open");
    anim_player.set_loop(true);
}

void Mimic::c_reset() {
    anim_player.stop();
    MobileTrap::c_reset();
}