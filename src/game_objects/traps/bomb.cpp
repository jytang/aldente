#include "bomb.h"
#include "asset_loader.h"
#include "game/game_state.h"

Bomb::Bomb(int x, int z, int id) : Trap(x, z, id) {
    tag = "BOMB";

    if (id == ON_SERVER) {
        //Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_box;
        rigid.is_ghost = true;
        rigid.position = { x, 0.0f, z };
        events::add_rigidbody_event(rigid);

        blast_radius = 4.0f;
    }
}

void Bomb::s_on_collision(GameObject *other) {

    // Can't explode twice
    if (exploded)
        return;

    // Explode if stepped on by a player
    Player *player = dynamic_cast<Player*>(other);
    if (player) {
        s_explode();
        events::dungeon::network_collision_event(player->get_id(), id);
        exploded = true;
    }
}

void Bomb::c_on_collision(GameObject *other) {
    if (!exploded) {
        c_explode();
        exploded = true;
    }

    Player* player = dynamic_cast<Player*>(other);
    if (player)
        player->c_take_damage();
}

void Bomb::s_reset() {
    exploded = false;
    enable();
}

void Bomb::c_reset() {
    exploded = false;
    model->reset_colors();
    setup_model();
}

void Bomb::setup_model() {
    // Setup initially as mine
    attach_model(AssetLoader::get_model("mine"));
    set_scale({ 0.005f, 0.005f, 0.005f });
}

// Damages all player within blast radius
void Bomb::s_explode() {
    for (auto pair : GameState::players) {
        Player *player = pair.second;
        if (player) {
            if (glm::distance(player->transform.get_position(), transform.get_position()) <= blast_radius) {
                player->s_take_damage();
                // Send signal to client that this player was hit
                events::dungeon::network_collision_event(player->get_id(), id);
            }
        }
    }

    // Disables after animation ends
    Timer::get()->do_after(std::chrono::milliseconds(1000), 
        [&]() {
        disable();
    });
}

// Switch to explosion animation
void Bomb::c_explode() {
    attach_model(AssetLoader::get_model("explosion"));
    set_scale({ 0.005f, 0.005f, 0.005f });

    // Make Glow and Transparent
    model->multiply_colors({ 2.0f, 2.0f, 2.0f, false });
    model->set_alpha(0.8f);
    
    anim_player.set_anim("go", 1.0f, false);
    anim_player.play();
}