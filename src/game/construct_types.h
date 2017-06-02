#pragma once

// Type of construct built on tile.
enum ConstructType {
    NONE, CHEST, MIMIC, SPIKES, SLIME_B, SLIME_Y, SLIME_R, SLIME_G, ARROW_TRAP, ICE, REMOVE
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
        { ConstructType::MIMIC, { ConstructType::MIMIC, 50, "Mimic", "A treasure chest with the urge to kill.", "no.png" } },
        { ConstructType::SPIKES, { ConstructType::SPIKES, 50, "Spikes", "It was me, spikes.", "dio.jpg" } },
        { ConstructType::SLIME_B, { ConstructType::SLIME_B, 10, "Blue Slime", "Likes to loop", "slime_blue.png" } },
        { ConstructType::SLIME_Y,{ ConstructType::SLIME_Y, 10, "Yellow Slime", "Bounces off walls", "slime_yellow.png" } },
        { ConstructType::SLIME_R,{ ConstructType::SLIME_R, 10, "Red Slime", "Pretty straightforward", "slime_red.png" } },
        { ConstructType::SLIME_G,{ ConstructType::SLIME_G, 10, "Green Slime", "Unpredictable little bastard", "slime_green.png" } },
        { ConstructType::ARROW_TRAP,{ ConstructType::ARROW_TRAP, 10, "Arrow", "Sharp as a daisy.", "cloudwall.png" } },
        { ConstructType::ICE,{ ConstructType::ICE, 10, "Ice Trap", "Gets pretty chilly.", "Kororo.jpg" } },
        { ConstructType::REMOVE, { ConstructType::REMOVE, 200, "Remove", "Remove a block.", "Tomato.jpg" } }
    };
}