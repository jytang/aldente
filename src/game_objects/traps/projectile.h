#pragma once

#include "game_objects/game_object.h"
#include "timer.h"

/*
    Projectiles that continuousl moves forward until it either collides
    with an object, or times out. Usually carried around by a game object.
*/
class Projectile : public GameObject {
private:
    std::function<void()> cancel_timer;
protected:
    float speed = 1.0f;
public:
    Projectile(int id = 0);

    // Fires Projectile towards given direction
    void fire(Transform spawn_location);

    // Moves forward
    void handle_movement();

    // Sets up timer for when this object is timed out
    void setup_timer(long long time_out_ms);

    virtual void s_update_this();

    virtual void s_on_collision(GameObject *other) override;
    virtual void c_on_collision(GameObject *other) override;

    virtual void play_trigger_animation() {};
};