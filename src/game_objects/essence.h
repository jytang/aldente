#pragma once

#include "game_object.h"
#include "events.h"
#include "game/collectibles/collectible.h"

#define DEFAULT_ESSENSE_VAL 10

/*
    Dropped objects that players can pick up for currency
*/
class Essence : public GameObject {
public:
    Essence(int id = 0);

    void s_update_this() override;

    void s_on_collision(GameObject *other) override;
    void c_on_collision(GameObject *other) override;

    void setup_model() override;

    void random_push();
    void disappear();

private:
    btSphereShape *hit_sphere = new btSphereShape(0.2f);

    std::unique_ptr<collectibles::Collectible> value;

    std::function<void()> cancel_fade;
    std::function<void()> cancel_rainbow; // Cancels the constant rainbow color changing

    bool disappearing;

    void pickup_anim();
    void setup_timeout();

    void disable_after_disappear();
};