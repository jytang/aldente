#include "construct.h"
#include "asset_loader.h"
#include "player.h"
#include "timer.h"
#include "game/collectibles/gold.h"
#include "game/collectibles/nothing.h"

Construct::Construct(int x, int z, int id) : GameObject(id) {
    tag = "CONSTRUCT";
    set_position({ x, 0.0f, z });
    initial_transform.set_position({ x, 0.0f, z });
}

/************CHEST***************/

Chest::Chest(int x, int z, int id) : Construct(x, z, id),
        cancel_disappear([]() {}), cancel_fade([]() {}) {
    tag = "CHEST";

    events::dungeon::s_prepare_dungeon_event.connect([&]() {
        // Fill chest with gold
        contents = std::make_unique<collectibles::Gold>(500);
    });

    if (id == ON_SERVER) {
        //Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_box;
        rigid.position = { x, 0.0f, z };
        collision_group = COLLISION_STRUCTS;
        events::add_rigidbody_event(rigid);
    }
}

void Chest::s_interact_trigger(GameObject *other) {

    if (opened)
        return;

    // Check if other is a player, than grant some money
    Player *player = dynamic_cast<Player*>(other);
    if (player) {
        contents->collected_by(player);

        // Remove the item once collected
        contents = std::make_unique<collectibles::Nothing>();
    }

    // Disables objects once animation finishes
    cancel_disappear = Timer::get()->do_after(std::chrono::milliseconds(1500), [&]() {
        disable();
    });

    // Send signal to client to tell that this chest is opened
    events::dungeon::network_interact_event(other->get_id(), id);

    opened = true;
}

// Activated when a player presses A on it, graphical
void Chest::c_interact_trigger(GameObject *other) {
    events::sound_effects_event(events::AudioData(AudioManager::CHEST_OPENING_SOUND, false));

    anim_player.play();

    cancel_disappear = Timer::get()->do_after(std::chrono::milliseconds(1000),
        [&]() {
        disappear();
    });
}

void Chest::setup_model() {
    attach_model(AssetLoader::get_model("chest_good"));
    set_scale({ 0.006f, 0.006f, 0.006f });
    anim_player.set_anim("open", 1.5f, false);
}

// Causes chest to slowly fade away, Client
void Chest::disappear() {
    int count = 0;
    cancel_fade = Timer::get()->do_every(
        std::chrono::milliseconds(50),
        [&, count]() mutable {
        const float num_steps = 20;
        if (count <= num_steps) {
           set_filter_alpha(1.f - (count / num_steps));
        }
        else {
            anim_player.stop();
            cancel_fade();
        }
        count++;
    });
}

void Chest::s_reset() {
    cancel_disappear();
    opened = false;
    enable();
}

void Chest::c_reset() {
    cancel_disappear();
    cancel_fade();
    anim_player.stop();
    set_filter_alpha(1.0f);
    enable();
}

/************GOAL***************/
Goal::Goal(int x, int z, int id) : Construct(x, z, id) {
    tag = "GOAL";
    play_buzz = false;

    if (id == ON_SERVER) {
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = goal_hit_box;
        rigid.is_ghost = true;
        rigid.position = { x, 0.5f, z };
        events::add_rigidbody_event(rigid);

        notify_on_collision = true;
    }

    events::dungeon::enable_goal_buzz.connect([&]() {
        play_buzz = true;
    });

    events::dungeon::disable_goal_buzz.connect([&]() {
        play_buzz = false;
    });
}

void Goal::setup_model() {
    attach_model(AssetLoader::get_model("warp"));
    set_scale({ 0.006f, 0.006f, 0.006f });
    anim_player.set_anim("spin", 1.0f, true);
    anim_player.play();
}

void Goal::s_on_collision(GameObject *other) {
    Player *player = dynamic_cast<Player*>(other);

    if (player && !player->get_exiting_status()) {
        player->s_begin_warp(transform.get_position().x, transform.get_position().z);
        events::dungeon::network_collision_event(other->get_id(), id);
    }
}

void Goal::c_on_collision(GameObject *other) {
    Player *player = dynamic_cast<Player*>(other);
    if (player && !player->get_exiting_status()) {
        player->c_begin_warp();
    }
}

void Goal::c_update_this() {
    if (play_buzz)
        events::sound_effects_event(events::AudioData(AudioManager::PORTAL_BUZZ_SOUND, false, GameState::context.client_player, this));
}
