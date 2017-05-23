#include "construct.h"
#include "asset_loader.h"
#include "player.h"
#include "timer.h"
#include "game/collectibles/gold.h"
#include "game/collectibles/nothing.h"

Construct::Construct(int x, int z, int id) : GameObject(id) {
    tag = "CONSTRUCT";
    set_position({ x, 0.0f, z });
}

/************CHEST***************/

Chest::Chest(int x, int z, int id) : Construct(x, z, id) {
    tag = "CHEST";

    events::dungeon::s_prepare_dungeon_event.connect([&]() {
        // Fill chest with gold
        contents = std::make_unique<collectibles::Gold>(10);
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

    // Send signal to client to tell that this chest is opened
    events::dungeon::network_interact_event(other->get_id(), id);
}

// Activated when a player presses A on it, graphical
void Chest::c_interact_trigger(GameObject *other) {
    anim_player.play_if_paused("open");
}

void Chest::setup_model() {
    attach_model(AssetLoader::get_model("chest_good"));
    transform.set_scale({ 0.006f, 0.006f, 0.006f });
}

/************SPIKES***************/

Spikes::Spikes(int x, int z, int id) : Construct(x, z, id) {
    tag = "SPIKES";

    if (id == ON_SERVER) {
        //Creates Rigid Body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_box;
        rigid.is_ghost = true;
        rigid.position = { x, 0.0f, z };
        events::add_rigidbody_event(rigid);
        notify_on_collision = true;
    }
}

void Spikes::s_on_collision(GameObject *other) {
    Player *player = dynamic_cast<Player*>(other);
    if (player) {
        if (player->s_take_damage()) {
            // Send signal to client that this player was hit
            events::dungeon::network_collision_event(other->get_id(), id);
        }
    }
}

void Spikes::c_on_collision(GameObject *other) {
    anim_player.play_if_paused("trigger");

    Player* player = dynamic_cast<Player*>(other);
    assert(player);
    player->c_take_damage();
}

void Spikes::setup_model() {
    attach_model(AssetLoader::get_model("spikes"));
    transform.set_scale({ 0.4f, 0.4f, 0.4f });
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
    model->set_shader(&ShaderManager::anim_unlit);
    anim_player.set_speed(1.0f);
    anim_player.set_anim("spin");
    anim_player.set_loop(true);
    anim_player.play();
}

void Goal::s_on_collision(GameObject *other) {
    Player *player = dynamic_cast<Player*>(other);

    if (player && player->is_enabled()) {
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
