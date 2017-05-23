#include "trap.h"
#include "game_objects/game_object.h"

Trap::Trap(int x, int z, int id) : Construct(x, z, id) {
    tag = "TRAP";
}