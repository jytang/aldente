#pragma once

#include "projectile_trap.h"

class ArrowTrap : public ProjectileTrap {
private:
    btBoxShape *hit_box = new btBoxShape(btVector3(0.4f, 0.4f, 0.4f));
public:
    ArrowTrap(int x, int z, int id = 0);
};