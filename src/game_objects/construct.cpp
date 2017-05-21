#include "construct.h"
#include "asset_loader.h"
#include "player.h"

Construct::Construct(int x, int z, int id) : GameObject(id) {
    tag = "CONSTRUCT";
    set_position({ x, 0.0f, z });
}

void Construct::c_update_state(float x, float z, float wx, float wz) {
    anim_player.update();

    GameObject::c_update_state(x, z, wx, wz);
}

/************CHEST***************/

Chest::Chest(int x, int z, int id) : Construct(x, z, id) {
    tag = "CHEST";

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

    // Send signal to client to tell that this chest is opened
    events::dungeon::network_interact_event(id);
}

// Activated when a player presses A on it, graphical
void Chest::c_interact_trigger() {
    anim_player.set_anim("open");
    anim_player.play();
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
    }
}

void Spikes::s_on_collision(GameObject *other) {
    Player *player = dynamic_cast<Player*>(other);
    if (player) {
        events::dungeon::network_collision_event(id);
    }
}

void Spikes::c_on_collision() {
    anim_player.set_anim("trigger");
    anim_player.play();
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
        events::dungeon::network_collision_event(id);
        events::dungeon::player_finished_event(player->get_id());
    }
}
