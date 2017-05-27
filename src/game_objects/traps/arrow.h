#pragma once

#include "projectile.h"

/*
    Projectile shot out of an Arrow Trap.
*/
class Arrow : public Projectile {
private:
    // TODO : Properly adjust hit box
    btBoxShape *hit_box = new btBoxShape(btVector3(0.2f, 0.2f, 0.4f));
public:
    Arrow(int id = 0);

    void play_trigger_animation() override;
    void setup_model() override;
};