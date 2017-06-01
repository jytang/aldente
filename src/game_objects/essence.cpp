#include "essence.h"
#include "events.h"
#include "asset_loader.h"
#include "player.h"
#include "util/util_bt.h"
#include "timer.h"
#include "util/util.h"
#include "game/collectibles/gold.h"
#include "game/collectibles/nothing.h"

#include "collision_groups.h"

#include <iostream>

#define ESSENCE_TIME_OUT 6000 // Milliseconds
#define DISAPPEAR_ANIM_LENGTH 500 // Time it takes for disappearing animation to finish

Essence::Essence(int id) : GameObject(id){
    tag = "ESSENCE";

    // Set Value
    value = std::make_unique<collectibles::Gold>(DEFAULT_ESSENSE_VAL);

    if (id == ON_SERVER) {
        // Setup rigid body
        events::RigidBodyData rigid;
        rigid.object = this;
        rigid.shape = hit_sphere;
        rigid.mass = 0.1f;
        rigid.collision_group = COLLISION_ESSENCE;
        rigid.collision_mask |= COLLISION_ESSENCE; // don't collide with own kind
        events::add_rigidbody_event(rigid);
        notify_on_collision = true;

        // Lock y-axis
        rigidbody->setLinearFactor(btVector3(1, 0.0f, 1));

        random_push();

        setup_timeout();
    }
    else {
        // Make the essence change colors continuously
        int num_steps = 200;
        int count = (int) Util::random(0, num_steps * 2);
        cancel_rainbow = Timer::get()->do_every(
            std::chrono::milliseconds(10),
            [&, num_steps, count]() mutable{
            const float frequency = (2 * glm::pi<float>()) / num_steps;
            float r, g, b;

            // For smooth looping
            int i = count;
            if (count > num_steps)
                i = num_steps * 2 - count;

            // Adjust rgb values based on frequencies, the last number dictates the 'balance' (0 - 255)
            r = sinf(frequency * i + 0) * 1100 + 1600; // These numbers were eyeballed visually
            g = sinf(frequency * i + 2) * 1100 + 1600;
            b = sinf(frequency * i + 4) * 1100 + 1600;

            set_filter_color({ r, g, b});

            count = (count < num_steps * 2) ? count + 1 : 0;
        });
    }
}

void Essence::s_update_this() {

    // Get the transform from Bullet and update
    btTransform t;
    rigidbody->getMotionState()->getWorldTransform(t);
    btVector3 to_set = t.getOrigin();
    transform.set_position(util_bt::convert_vec3(to_set));
}

void Essence::s_on_collision(GameObject *other) {
    if (disappearing)
        return;

    Player *player = dynamic_cast<Player*>(other);
    if (player && !player->is_stunned()) {
        value->collected_by(player); // Award player
        value = std::make_unique<collectibles::Nothing>();

        disappearing = true;

        events::dungeon::network_collision_event(player->get_id(), id);

        // Object becomes a ghost, so not moved while disappearing.
        set_ghost(true);

        disable_after_disappear();
    }
}

void Essence::c_on_collision(GameObject *other) {
    if (dynamic_cast<Player*>(other))
        pickup_anim();
    disappear();
}

void Essence::setup_model() {
    attach_model(AssetLoader::get_model("essence"));
    transform.set_scale({ 0.005f, 0.005f, 0.005f });
    initial_transform.set_scale(transform.get_scale());

    // Coin always spins
    anim_player.set_anim("spin");
    anim_player.set_speed(Util::random(0.4f, 2.0f));
    anim_player.set_loop(true);
    anim_player.play();
}

void Essence::disappear() {
    int count = 0;
    // Slowly fade away
    cancel_fade = Timer::get()->do_every(
        std::chrono::milliseconds(50),
        [&, count]() mutable {
        const float max_count = 10;
        if (count < max_count) {
            set_filter_alpha(1.0f - (count / max_count));
            transform.translate({ 0, 0.02f, 0 }); // Bounce up animation
        }
        else {
            cancel_fade();
        }

        count++;
    });
}

void Essence::random_push() {
    // Setup Initial Push in Random direction
    glm::vec2 vel = glm::vec2(Util::random(-1.f, 1.f), Util::random(-1.f, 1.f));
    vel = vel * 100.f; // Impulse Force

    rigidbody->setActivationState(true);
    rigidbody->applyForce(btVector3(vel.x, 0, vel.y), btVector3(0, 0, 0));
    rigidbody->setDamping(0.9, 0);
}

void Essence::pickup_anim() {
    // Add "pick up" animation
    anim_player.set_anim("up");
    anim_player.set_speed(3.0f);
    anim_player.set_loop(false);
    anim_player.play();
}

void Essence::setup_timeout() {
    // Make essence disappear after a while.
    int time_out = ESSENCE_TIME_OUT + (int)Util::random(-1000, 1000); // Slightly diff time outs
    Timer::get()->do_after(
        std::chrono::milliseconds(time_out),
        [&]() mutable {
        // Tells client to start disappearing animation. This is the easiest way to send animation commands to client atm.
        events::dungeon::network_collision_event(id, id);
        disable_after_disappear();
    });
}

// Server side function that times the disable to happen as soon as the client side animation ends.
void Essence::disable_after_disappear() {
    // Call disable() once client side disappearing animation ends
    Timer::get()->do_after(
        std::chrono::milliseconds(DISAPPEAR_ANIM_LENGTH),
        [&]() {
        disable();
    });
}