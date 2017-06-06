#include "shooter.h"
#include "asset_loader.h"
#include "game/game_state.h"

Shooter::Shooter(int x, int z, int id) : ProjectileTrap(x, z, id) {
    if (id == ON_SERVER) {
        // Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_box;
        rigid.position = { x, 0.0f, z };
        rigid.is_ghost = true;
        collision_group = COLLISION_STRUCTS;
        events::add_rigidbody_event(rigid);

        fill_projectile_pool(ProjectileTypes::ARROW);

        attack_range = 2.0f;
        activation_type = AI;
    }
}

void Shooter::setup_model() {
    Model *model = AssetLoader::get_model("arrow_trap");
    attach_model(model);
    set_scale({ 0.0095f, 0.0095f, 0.0095f });

    anim_player.set_anim("shoot");
    anim_player.set_loop(false);
    anim_player.set_speed(5.0f);
}

void Shooter::play_trigger_animation() {
    anim_player.play();
}


void Shooter::update_ai() {
    
    if (curr_target) {
        // If have a target, check if within attack range and shoot
        attempt_attack(curr_target);
    }
    else {
        // If no target, finds a new target within range
        for (auto pair : GameState::players) {
            Player * player = pair.second;
            if (in_range(player)) {
                curr_target = player;
                attempt_attack(player);
                break;
            }
        }
    }
}

void Shooter::attempt_attack(Player *player) {
    if (in_range(player)) {
        turn_to(player);
        shoot();
    }
}

// Checks if the player is within attack range
bool Shooter::in_range(Player *player) {
    return (glm::distance(player->transform.get_position(),
        transform.get_position()) < attack_range);
}

// Turn to face towards the player
void Shooter::turn_to(Player *player) {
    transform.look_at(player->transform.get_position() - transform.get_position());
}