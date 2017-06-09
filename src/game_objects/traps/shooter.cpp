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

        fill_projectile_pool(ProjectileTypes::SPHERE);

        attack_range = 50.0f;
        activation_type = AI;
    }
}

void Shooter::setup_model() {
    Model *model = AssetLoader::get_model("minion");
    attach_model(model);
    set_scale({ 0.005f, 0.005f, 0.005f });

    anim_player.set_anim("shoot");
    anim_player.set_loop(false);
    anim_player.set_speed(1.0f);
}

void Shooter::play_trigger_animation() {
    anim_player.play();
}


void Shooter::update_ai() {
    
    if (curr_target) {
        // If have a target, check if within attack range and shoot
        if (in_range(curr_target)) {
            attack(curr_target);

            // update whether or not target is still in sight
            check_sight(curr_target);
        }
        else
            curr_target = nullptr;
    }
    else {
        // If no target, finds a new target within range
        for (auto pair : GameState::players) {
            Player * player = pair.second;
            if (in_range(player) && !curr_target) {
                check_sight(player);
            }
        }
    }
}

void Shooter::attack(Player *player) {
    turn_to_shoot(player);
    shoot();
}

// Checks if the player is within line of sight using a raycast
void Shooter::check_sight(Player *player) {
    glm::vec3 dir = player->transform.get_position() - transform.get_position();
    events::dungeon::request_raycast_event(
        transform.get_position(), dir, attack_range,
        [&](GameObject *bt_hit) {
        Player *player = dynamic_cast<Player*>(bt_hit);
        if (player)
            curr_target = player;
        else
            curr_target = nullptr;
    });
}

// Checks if the player is within attack range
bool Shooter::in_range(Player *player) {
    return (glm::distance(player->transform.get_position(),
        transform.get_position()) < attack_range);
}

// Turn to face towards the player and shoots when finished turning
void Shooter::turn_to_shoot(Player *player) {

    glm::vec3 orig = transform.get_position();
    glm::vec3 goal = player->transform.get_position();

    transform.look_at(goal - orig, 0.3f);
}