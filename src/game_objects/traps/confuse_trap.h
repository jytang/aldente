#pragma once

#include "trap.h"

/*
    A tile-based trap. Freezes the player, and then slowly regains their mvoespeeed
*/
class SlowTrap : public Trap {
public:
    SlowTrap(int x, int z, int id = 0);

    void s_on_collision(GameObject *other) override;
    void c_on_collision(GameObject * other) override;

    void setup_model() override;
private:
    btBoxShape *hit_box = new btBoxShape(btVector3(0.25f, 0.25f, 0.25f));
};