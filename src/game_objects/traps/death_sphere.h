#pragma once

#include "projectile.h"

/*
    Projectile shot out of by Minion shooter.
*/
class DeathSphere : public Projectile {
private:
    btSphereShape *hit_sphere = new btSphereShape(0.2f);
public:
    DeathSphere(int id = 0);

    void play_trigger_animation() override;
    void setup_model() override;
};