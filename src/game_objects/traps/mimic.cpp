#include "mimic.h"
#include "asset_loader.h"
#include "util/path_finding.h"

Mimic::Mimic(int x, int z, int id) : MobileTrap(x, z, id) {
    if (id == ON_SERVER) {
        // Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_capsule; // To avoid getting stuck at edges
        rigid.mass = 2;
        rigid.is_ghost = false;
        rigid.position = { x, 0.0f, z };
        collision_group = COLLISION_TRAPS;
        events::add_rigidbody_event(rigid);

        // Slightly random movespeed to prevent complete overlap of mimics
        move_speed = Util::random(3.5f, 4.5f);
        move_type = AI;

        s_to_chest_mode();
    }
}

// Handles logic for whether the AI is still in range of the target
void Mimic::update_ai() {
    if (curr_target) {
        // First check if can go straight to target (Raycast test first since less expensive than path finding)
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
        curr_target = player; // Targets Player
        s_to_attack_mode();
        // Send signal to client to tell that this chest is opened
        events::dungeon::network_interact_event(other->get_id(), id);
    }
}

void Mimic::c_interact_trigger(GameObject *other) {
    c_to_mimic();
}

void Mimic::setup_model() {
    c_to_chest();
}

void Mimic::s_reset() {
    s_to_chest_mode();
    MobileTrap::s_reset();
}

void Mimic::c_reset() {
    anim_player.stop();
    c_to_chest();
    MobileTrap::c_reset();
}

// Switch to immovable chest
void Mimic::s_to_chest_mode() {
    curr_target = NULL;
    stop_moving = true;
    notify_on_collision = false;
    rigidbody->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
}

// Switch to player-targeting monster
void Mimic::s_to_attack_mode() {
    stop_moving = false; // Can Now move
    notify_on_collision = true; // Can now damage player on collision
    rigidbody->setLinearFactor(btVector3(1, 0.0f, 1)); // Can move normally
}

void Mimic::c_to_chest() {
    Model *model = AssetLoader::get_model("chest_bad");
    attach_model(model);
    set_scale({ 0.006f, 0.006f, 0.006f }); // original scale
}

void Mimic::c_to_mimic() {
    Model *model = AssetLoader::get_model("mimic");
    attach_model(model);

    // Grows a bit bigger as a mimic
    set_scale({ 0.0062f, 0.0062f, 0.0062f });

    anim_player.set_anim("walk", 3.0f, true);
    anim_player.play();
}