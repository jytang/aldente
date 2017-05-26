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

Chest::Chest(int x, int z, int id) : Construct(x, z, id) {
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
        events::add_rigidbody_event(rigid);
    }
}

void Chest::s_interact_trigger(GameObject *other) {
    // Check if other is a player, than grant some money
    Player *player = dynamic_cast<Player*>(other);
    if (player) {
        contents->collected_by(player);

        // Remove the item once collected
        contents = std::make_unique<collectibles::Nothing>();
    }

    // Chest will fade away, no longer needs rigid body
    // TODO : Change to Disable() once Client Animation Finishes
    Timer::get()->do_after(std::chrono::milliseconds(1500), [&]() {
        disable();
    });

    // Send signal to client to tell that this chest is opened
    events::dungeon::network_interact_event(other->get_id(), id);
}

// Activated when a player presses A on it, graphical
void Chest::c_interact_trigger(GameObject *other) {
    anim_player.play_if_paused("open");

    Timer::get()->do_after(std::chrono::milliseconds(500),
        [&]() {
        disappear();
    });
}

void Chest::setup_model() {
    attach_model(AssetLoader::get_model("chest_good"));
    transform.set_scale({ 0.006f, 0.006f, 0.006f });
    initial_transform.set_scale(transform.get_scale());
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
    enable();
}

void Chest::c_reset() {
    set_filter_alpha(1.0f);
    enable();
}

/************GOAL***************/
Goal::Goal(int x, int z, int id) : Construct(x, z, id) {
    tag = "GOAL";

    if (id == ON_SERVER) {
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = goal_hit_box;
        rigid.is_ghost = true;
        rigid.position = { x, 0.5f, z };
        events::add_rigidbody_event(rigid);

        notify_on_collision = true;
    }
}

void Goal::setup_model() {
    attach_model(AssetLoader::get_model("warp"));
    transform.set_scale(0.006f, 0.006f, 0.006f);
    initial_transform.set_scale(transform.get_scale());
    anim_player.set_speed(1.0f);
    anim_player.set_anim("spin");
    anim_player.set_loop(true);
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
