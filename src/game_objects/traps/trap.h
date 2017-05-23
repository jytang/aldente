#pragma once

#include "game_objects/construct.h"

/*
    For organizing "traps" into a shared base class
*/
class Trap : public Construct {
public:
    Trap(int x, int z, int id = 0);
};