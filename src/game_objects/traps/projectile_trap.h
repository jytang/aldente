#pragma once

#include "trap.h"

/*
    Traps that spawn another game object and uses its child's collider to
    damage the player.
*/
class ProjectileTrap : public Trap {
public:
    ProjectileTrap(int x, int z, int id = 0);
};