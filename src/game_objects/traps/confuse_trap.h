#pragma once

#include "trap.h"

/*
    A tile-based trap. Confuses the player, reversing their movement
*/
class ConfuseTrap : public Trap {
public:
    ConfuseTrap(int x, int z, int id = 0);

    void s_on_collision(GameObject *other) override;
    void c_on_collision(GameObject * other) override;

    void setup_model() override;
private:
    btBoxShape *hit_box = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
};