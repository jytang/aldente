#pragma once

#include "game_objects/game_object.h"
#include "timer.h"

enum ProjectileTypes {
    ARROW,
};

/*
    Projectiles that continuousl moves forward until it either collides
    with an object, or times out. Usually carried around by a game object.
*/
class Projectile : public GameObject {
private:
    std::function<void()> cancel_timer;
protected:
    float speed = 5.0f;
    long long time_out_ms = 2000; // Time it takes before projectile disappears, in ms
    ProjectileTypes projectile_type;
    int parent_id;

    float height_offset = 0.5f;
public:
    Projectile(int id = 0);

    // Fires Projectile towards given direction
    void fire(Transform spawn_location);

    // Moves forward
    void handle_movement();

    // Sets up timer for when this object is timed out
    void setup_timer();

    virtual void s_update_this();

    virtual void s_on_collision(GameObject *other) override;
    virtual void c_on_collision(GameObject *other) override;

    virtual void play_trigger_animation() {};

    // Type of Projectile
    int get_type() { return projectile_type; };

    // Parent Id for properly attaching Trap to Projectile
    void set_parent_id(int p_id) { parent_id = p_id; };
    int get_parent_id() { return parent_id; };

    // For attaching the projectile as a child to the given parent id
    void set_parent(int p_id);
};