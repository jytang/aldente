#pragma once

// Header only to globally define collision groups

#define BIT(x) (1<<(x))

// ONLY 15 FILTERS SUPPORTED!
enum CollisionGroup {
    COLLISION_NONE = 0,
    COLLISION_DEFAULT = BIT(0),
    COLLISION_PLAYERS = BIT(1),
    COLLISION_ESSENCE = BIT(2),
    COLLISION_STRUCTS = BIT(3),
    COLLISION_TRAPS = BIT(4)
};