#pragma once

// Type of construct built on tile.
enum ConstructType {
    NONE, CHEST, SPIKES, SLIME, REMOVE
};

struct ConstructData {
    ConstructType type;
    int cost;
    std::string name;
    std::string description;
    std::string image;
};

namespace Constructs {
    const static std::map<ConstructType, ConstructData> CONSTRUCTS = {
        { ConstructType::CHEST, { ConstructType::CHEST, 50, "Chest", "A treasure chest full of gold.", "test.png" } },
        { ConstructType::SPIKES, { ConstructType::SPIKES, 50, "Spikes", "It was me, spikes.", "dio.jpg" } },
	{ ConstructType::SLIME, { ConstructType::SLIME, 50, "Slime", "Squishy", "slime.png" } },
        { ConstructType::REMOVE, { ConstructType::REMOVE, 200, "Remove", "Remove a block.", "Tomato.jpg" } }
    };
}
