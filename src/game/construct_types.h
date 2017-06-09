#pragma once

// Type of construct built on tile.
enum ConstructType {
    NONE, CHEST, MIMIC, BOMB, SPIKES, SLIME_B, SLIME_Y, SLIME_R, SLIME_G, ARROW_TRAP, SHOOTER, ICE, CONFUSE, REMOVE
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
        { ConstructType::CHEST,      { ConstructType::CHEST,        50, "Chest",        "Lucrative cache of dream essence",     "chest_icon.png" } },
        { ConstructType::MIMIC,      { ConstructType::MIMIC,        50, "Mimic",        "Chest of ceaseless killer intent",     "mimic_icon.png" } },
        { ConstructType::BOMB,       { ConstructType::BOMB,         10, "Mine Bomb",    "Blesses the stepper with a blast",     "bomb_icon.png" } },
        { ConstructType::SPIKES,     { ConstructType::SPIKES,       50, "Spikes",       "Essence extractor, impaler style",     "spike_icon.png" } },
        { ConstructType::SLIME_B,    { ConstructType::SLIME_B,      10, "Blue Slime",   "Aldente aschente grandma sandma!",     "slime_icon.png" } }, // change flavour here
        { ConstructType::SLIME_Y,    { ConstructType::SLIME_Y,      10, "Bunny",        "Bouncy, fluffy, and also painful",     "bunny_icon.png" } },
        { ConstructType::SLIME_R,    { ConstructType::SLIME_R,      10, "Bad Shroom",   "Volatile shroom, of the bad sort",     "badmush_icon.png" } },
        { ConstructType::SLIME_G,    { ConstructType::SLIME_G,      10, "Green Slime",  "One unpredictable little bastard",     "slime_icon.png" } },
        { ConstructType::ARROW_TRAP, { ConstructType::ARROW_TRAP,   10, "Arrow",        "Mindlessly barrages where it faces",   "arrow_icon.png" } },
        { ConstructType::SHOOTER,    { ConstructType::SHOOTER,      10, "Devil",        "Sentient sentry, deviruchi devil",     "devil_icon.png" } },
        { ConstructType::ICE,        { ConstructType::ICE,          10, "Ice",          "50% slow movement speed/attack speed", "ice_icon.png" } },
        { ConstructType::CONFUSE,    { ConstructType::CONFUSE,      10, "Confuse",      "Muddles mind, disorients direction",   "spiral.png" } },
        { ConstructType::REMOVE,     { ConstructType::REMOVE,       20, "Remove",       "Remove a block.",                      "no_player.png" } },
    };
}