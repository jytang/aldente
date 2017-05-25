#pragma once

#include "game_objects/traps/trap.h"

/*
    Collision-based traps, which damages players when the player collide's with the
    object's box collider.
*/
class CollisionTrap : public Trap {
public:
    CollisionTrap(int x, int z, int id = 0);

    virtual void s_on_collision(GameObject *other) override;
    virtual void c_on_collision(GameObject *other) override;

    // Plays triggering animation, if object has one
    virtual void play_trigger_animation() {};
};