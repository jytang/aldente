#include "player.h"
#include "audio/audio_manager.h"
#include "asset_loader.h"
#include "assert.h"
#include "events.h"
#include "timer.h"

#include "util/util.h"

#define ANIMATE_DELTA 0.001f
#define STUN_LENGTH 1000 // milliseconds
#define INVULNERABLE_LENGTH 3000 // ms
#define SLOW_LENGTH 5000

#define BASE_MOVE_SPEED 2.0f

std::vector<std::string> Player::PLAYER_MODELS = { "boy_two", "lizard", "cat", "tomato" };

Player::Player(int id) : GameObject(id), is_client(false) {
    tag = "PLAYER";

    if (id == ON_SERVER) {
        to_moveX = 0;
        to_moveZ = 0;
        move_speed = BASE_MOVE_SPEED;
        exiting = false;

        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_capsule;
        rigid.mass = 1;
        collision_group = COLLISION_PLAYERS;
        events::add_rigidbody_event(rigid);

        // Notify on collision.
        notify_on_collision = true;

        // Lock y-axis
        rigidbody->setLinearFactor(btVector3(1, 0.0f, 1));

        //Lock angular rotation
        rigidbody->setAngularFactor(0);
    }

    events::menu::c_cycle_player_model_event.connect([&](bool forward) {
        if (!is_client) return;

        // Determine the next model index to switch to.
        int next_index = forward ?
            (model_index + 1) % PLAYER_MODELS.size() :
            model_index == 0 ?
                PLAYER_MODELS.size() - 1 :
                (model_index - 1) % PLAYER_MODELS.size();

        // Send the request to the server.
        proto::ClientMessage msg;
        proto::AvatarChange* request = new proto::AvatarChange();
        request->set_model_index(next_index);
        request->set_player_id(get_id());
        msg.set_allocated_change_avatar_request(request);
        events::client::send(msg);
    });
}

// Just calls do_movement for now, can have more
// functionality later.
void Player::s_update_this() {
    if(!exiting)
        do_movement();

    sync_position();
}

void Player::prepare_movement(int inX, int inZ) {
    to_moveX = inX;
    to_moveZ = inZ;
}

void Player::c_update_state(glm::mat4 mat, bool enab) {
    anim_player.update();

    // Find difference in x and z positions for animating.
    float dx = std::fabs(mat[3][0] - transform.get_position().x);
    float dz = std::fabs(mat[3][2] - transform.get_position().z);
    bool animate = dx > ANIMATE_DELTA || dz > ANIMATE_DELTA;

    if (!animate && !exiting && !stunned) {
        if (!anim_player.check_paused()) {
            events::stop_sound_effects_event(AudioManager::FOOTSTEPS_SOUND);

            anim_player.stop();
        }
    }
    else {
        if (anim_player.check_paused()) {
            events::sound_effects_event(events::AudioData{ AudioManager::FOOTSTEPS_SOUND, 100, true });

            anim_player.play();
        }
    }

    GameObject::c_update_state(mat, enab);
}

void Player::do_movement() {

    if (stunned) {
        rigidbody->setLinearVelocity(btVector3(0, 0, 0));
        rigidbody->setActivationState(false);
        return;
    }

    // Should account for deltatime so movement is
    // framerate independent? Unsure how Bullet handles framerate.
    rigidbody->setActivationState(true);
    btVector3 vel = rigidbody->getLinearVelocity();
    rigidbody->setLinearVelocity(btVector3(to_moveX * move_speed, vel.getY(), to_moveZ * move_speed));
    transform.look_at(glm::vec3(to_moveX * move_speed, 0, to_moveZ * move_speed));
}

void Player::interact() {
    if (stunned)
        return;

    // Asks physics for a raycast to check if the player
    // is facing a construct.
    if (transform.get_forward().x != 0 || transform.get_forward().z != 0) {
        events::dungeon::request_raycast_event(
            transform.get_position(), transform.get_forward(), 0.6f,
            [&](GameObject *bt_hit) {
            Construct *construct = dynamic_cast<Construct*>(bt_hit);

            // Interacts with construct, construct itself will handle any client
            // side effects needed.
            if (construct) {
                construct->s_interact_trigger(this);
            }
        });
    }
}

void Player::stop_walk() {
    anim_player.stop();
}

void Player::start_walk() {
    anim_player.set_anim("walk", 3.0f, true);
}

// Server collision
void Player::s_on_collision(GameObject *other) {
    // By default, does not need to notify the client of any collisions.
    // If it triggers a trap, the trap will notify the client that the
    // player is hit. 
}

// Graphical collision
void Player::c_on_collision(GameObject *other) {
}

void Player::set_start_position(glm::vec3 pos) {
    initial_transform.set_position(pos);
}

void Player::reset_position() {
    transform = initial_transform;
    set_rb_position(initial_transform.get_position());
    to_moveX = 0;
    to_moveZ = 0;
}

void Player::c_setup_player_model(int index) {
    model_index = index;
    std::string model_name = PLAYER_MODELS[model_index];
    Model *player_model = AssetLoader::get_model(model_name);
    player_model->set_shader(&ShaderManager::unlit);
    attach_model(player_model);
    start_walk();

    // Sets scale. Need better way to do this later.
    if (model_name == "boy_two")
        set_scale({ 0.4f, 0.4f, 0.4f });
    else if (model_name == "cat")
        set_scale({ 0.004f, 0.004f, 0.004f });
    else if (model_name == "lizard")
        set_scale({ 0.0043f, 0.0043f, 0.0043f });
    else if (model_name == "tomato")
        set_scale({ 0.0043f, 0.0043f, 0.0043f });

    // Update leaderboard with player id and starting gold.
    // NOTE: THIS IS HERE BECAUSE LEADERBOARD WANTS TO KNOW WHICH MODEL TO ASSOCIATE.
    events::ui::leaderboard_update(id, stats.get_coins(), model_name);
}

void Player::s_begin_warp(float x, float z) {
    exiting = true;
    set_position({ x, 0, z });
    rigidbody->setLinearVelocity(btVector3(0, 0, 0));

    // Wait for animation before signaling phase
    Timer::get()->do_after(std::chrono::seconds(1), [&]() {
        events::dungeon::player_finished_event(id);
        exiting = false;
    });
}

void Player::c_begin_warp() {
    // Set up warp animation
    anim_player.set_anim("exit", 1.0f, true);
    anim_player.play();
    exiting = true;

    Timer::get()->do_after(std::chrono::seconds(1), [&]() {
        start_walk();
        exiting = false;
    });
}

bool Player::s_take_damage() {
    if (invulnerable)
        return false;

    // Period of invulerability
    invulnerable = true;

    // Period of stunned
    stunned = true;

    // Cannot collide with traps when invulnerable
    collision_mask |= COLLISION_TRAPS;
    events::update_collision_mask_event(this);

    // Player loses percentage essence
    const float percent_loss = .20f; // Hardcoded. Should change later to make it variable based on traps?
    int amount_loss = (int) stats.get_coins() * percent_loss;
    amount_loss = amount_loss - (amount_loss % 10); // Round down to nearest tenth
    amount_loss = (amount_loss <= 0 && stats.get_coins() > 0) ? 10 : amount_loss; // Loses a minimum of 10 essence
    s_modify_stats([&](PlayerStats & stats) {
        if (stats.get_coins() > 0)
            stats.add_coins(-amount_loss);
    });

    // Drop essence to total amount loss, rounded down. Assuming that each essence has 10 coin value. 
    const float essence_val = 10.0f; // Currently hardcoded
    int number_essence_loss = (int)floor(amount_loss / essence_val);
    for (int i = 0; i < number_essence_loss; i++)
        events::dungeon::s_spawn_essence_event(transform.get_position().x, transform.get_position().z);
    
    // End Stunned
    Timer::get()->do_after(std::chrono::milliseconds(STUN_LENGTH),
        [&]() {
        stunned = false;
    });

    // End Invulernability
    Timer::get()->do_after(std::chrono::milliseconds(INVULNERABLE_LENGTH),
        [&]() {
        invulnerable = false;

        // Can collide with traps again
        collision_mask = 0;
        events::update_collision_mask_event(this);
    });

    return true;
}

void Player::c_take_damage() {

    int count = 0;
    end_flicker = false;

    // Make sure all animations from previous cycle has ended
    if (cancel_flicker)
        cancel_flicker();
    if (cancel_stun)
        cancel_stun();
    if (cancel_invulnerable)
        cancel_invulnerable();

    // Flicker
    cancel_flicker = Timer::get()->do_every(
        std::chrono::milliseconds(100),
        [&, count]() mutable {
        if (count % 2)
            set_filter_alpha(1.0f);
        else
            set_filter_alpha(0.2f);

        if (end_flicker)
            disable_filter();

        count++;
    });

    // Change to the hurt animation
    stunned = true;
    anim_player.set_anim("damage", 1.0f, false);
    anim_player.play();

    // End hurt animation
    cancel_stun = Timer::get()->do_after(std::chrono::milliseconds(STUN_LENGTH),
        [&]() {
        stunned = false;
        start_walk();
    });

    // End
    cancel_invulnerable = Timer::get()->do_after(std::chrono::milliseconds(INVULNERABLE_LENGTH),
        [&]() {
        end_flicker = true;
        cancel_flicker();
    });
}

bool Player::s_slow() {
    if (slowed)
        return false;

    // Cannot be slowed forever
    slowed = true;

    // Start off unable to move, then slowly regain movespeed
    move_speed = 0;
    int count = 0;
    const int num_steps = 50;
    cancel_slow = Timer::get()->do_every(std::chrono::milliseconds(SLOW_LENGTH / num_steps),
        [&, count, num_steps]() mutable{
        move_speed += (float) BASE_MOVE_SPEED / (float) num_steps;

        if (move_speed >= BASE_MOVE_SPEED) {
            move_speed = BASE_MOVE_SPEED;
            cancel_slow();
            slowed = false;
        }

        count++;
    });

    return true;
}

void Player::c_slow() {
    if (cancel_slow)
        cancel_slow();

    // Turn player blue
    model->multiply_colors(Color(0.3f, 0.3f, 50.0f, false));

    // Fade back slowly to original color
    int count = 0;
    const int num_steps = 50;
    cancel_slow = Timer::get()->do_every(std::chrono::milliseconds(SLOW_LENGTH / num_steps),
        [&, count, num_steps]() mutable {

        model->reset_colors();

        float rg = Util::lerp(0.3f, 1.0f, (float)count / num_steps);
        float b = Util::lerp(50.0f, 1.0f, (float)count / num_steps);

        model->multiply_colors(Color(rg, rg, b, false));

        if (count >= num_steps) {
            cancel_slow();
            model->reset_colors();
        }

        count++;
    });
}

void Player::s_modify_stats(std::function<void(PlayerStats &)> modifier) {
    modifier(stats);

    // Dispatch an update to the clients
    proto::ServerMessage msg;
    auto *psu = msg.mutable_player_stats_update();
    psu->set_id(id);
    psu->set_coins(stats.get_coins());
    events::server::announce(msg);
}

void Player::c_update_stats(const proto::PlayerStats &update) {
    stats.set_coins(update.coins());

    if (is_client)
        events::c_player_stats_updated(update);

    // update leaderboard ui
    events::ui::leaderboard_update(id, update.coins(), PLAYER_MODELS[model_index]);
}

bool Player::can_afford(int cost) {
    return stats.get_coins() >= cost;
}

void Player::c_set_client_player() {
    is_client = true;
}

void Player::s_set_model_index(int index) {
    model_index = index;
}

int Player::c_get_model_index() const {
    return model_index;
}
