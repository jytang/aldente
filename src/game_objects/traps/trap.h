#pragma once

#include "game_objects/construct.h"

/*
    For organizing "traps" into a shared base class
    All constructs that do damage to a player is considered a trap. 
*/
class Trap : public Construct {
public:
    Trap(int x, int z, int id = 0) : Construct(x, z, id) {};
};