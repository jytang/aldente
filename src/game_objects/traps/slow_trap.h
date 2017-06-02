#pragma once

#include "trap.h"

/*
    A til-based trap which causes the player that steps over it to get slowed.
    Or maybe it freezes the player, and then slowly regains their mvoespeeed
    So maybe TODO : Rename to ice trap
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