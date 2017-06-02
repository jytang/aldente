#include "mimic.h"
#include "asset_loader.h"
#include "util/path_finding.h"

Mimic::Mimic(int x, int z, int id) : MobileTrap(x, z, id) {
    if (id == ON_SERVER) {
        // Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_capsule; // To avoid getting stuck at edges
        rigid.mass = 1000; // Cannot push around
        rigid.is_ghost = false;
        rigid.position = { x, 0.0f, z };
        events::add_rigidbody_event(rigid);

        // Lock y-axis
        rigidbody->setLinearFactor(btVector3(1, 0.0f, 1));

        move_speed = 4.0f;
        move_type = AI;

        stop_moving = true;
        notify_on_collision = false;
    }
}

// Handles logic for whether the AI is still in range of the target
void Mimic::update_ai() {
    if (curr_target) {
        // First check if can go straight to target
        glm::vec3 dir = glm::normalize(curr_target->transform.get_position() - transform.get_position());
        events::dungeon::request_raycast_event(
            transform.get_position(), dir, 100.0f,
            [&](GameObject *bt_hit) {
            Player *player = dynamic_cast<Player*>(bt_hit);

            // If something blocking path to player, use path finding.
            if (!player)
                dir = PathFinding::find_path(transform.get_position(), curr_target->transform.get_position());

            transform.look_at(dir);
        });
    }
}

// Check if other is a player, than targets them to follow them
void Mimic::s_interact_trigger(GameObject *other) {
    Player *player = dynamic_cast<Player*>(other);
    if (player && !curr_target) {
        curr_target = player;
        stop_moving = false;
        notify_on_collision = true; // Can now damage player on collision
        set_ghost(true);

        // Send signal to client to tell that this chest is opened
        events::dungeon::network_interact_event(other->get_id(), id);
    }
}

void Mimic::c_interact_trigger(GameObject *other) {
    anim_player.play();
}

void Mimic::setup_model() {
    Model *model = AssetLoader::get_model("chest_bad");
    attach_model(model);
    transform.set_scale({ 0.006f, 0.006f, 0.006f });
    initial_transform.set_scale(transform.get_scale());

    anim_player.set_anim("open", 3.0f, true);
}

void Mimic::s_reset() {
    curr_target = NULL;
    stop_moving = true;
    notify_on_collision = false;
    set_ghost(false);
}

void Mimic::c_reset() {
    anim_player.stop();
    MobileTrap::c_reset();
}