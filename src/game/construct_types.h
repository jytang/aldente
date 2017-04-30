#pragma once

// Type of construct built on tile.
enum ConstructType {
    CHEST, REMOVE, NONE
};

struct ConstructData {
    ConstructType type;
    int cost;
    std::string name;
    std::string description;
    std::string image;
};

namespace Constructs {
    static ConstructData chest = { ConstructType::CHEST, 50, "Chest", "A treasure chest full of gold.", "test.png" };
    static ConstructData remove = { ConstructType::REMOVE, 0, "Remove", "Remove a block.", "Tomato.jpg" };
}
