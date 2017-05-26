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
    float speed;
    long long time_out_ms; // In Milliseconds
public:
    Projectile(int id = 0);

    // Fires Projectile towards given direction
    void fire(int x, int z, int dir_x, int dir_z);

    // Moves forward
    void handle_movement();

    // Sets up timer for when this object is timed out
    void setup_timer();

    virtual void s_update_this();

    virtual void s_on_collision(GameObject *other) override;
    virtual void c_on_collision(GameObject *other) override;

    virtual void play_trigger_animation() {};
};