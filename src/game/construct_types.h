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
        { ConstructType::CHEST,      { ConstructType::CHEST,        50, "Chest",        "Lucrative cache of dream essence.",     "chest_icon.png" } },
        { ConstructType::MIMIC,      { ConstructType::MIMIC,        50, "Mimic",        "Chest of ceaseless killer intent.",     "mimic_icon.png" } },
        { ConstructType::BOMB,       { ConstructType::BOMB,         10, "Mine Bomb",    "So explosive, you'll be blown away.",     "bomb_icon.png" } },
        { ConstructType::SPIKES,     { ConstructType::SPIKES,       50, "Spikes",       "Quite pointy. Watch your step.",     "spike_icon.png" } },
        { ConstructType::SLIME_B,    { ConstructType::SLIME_B,      10, "Dumb Shroom",  "Deadly fungi that likes to walk in circles.",  "mush_icon.png" } },
        { ConstructType::SLIME_Y,    { ConstructType::SLIME_Y,      10, "Bunny",        "Bouncy, fluffy, and also painful.",     "bunny_icon.png" } },
        { ConstructType::SLIME_R,    { ConstructType::SLIME_R,      10, "Bad Shroom",   "A playful yet dangerous fungi.",     "badmush_icon.png" } },
        { ConstructType::SLIME_G,    { ConstructType::SLIME_G,      10, "Green Slime",  "A bit gooey, and not the good kind.",     "slime_icon.png" } },
        { ConstructType::ARROW_TRAP, { ConstructType::ARROW_TRAP,   10, "Arrow",        "Mindlessly barrages where it faces.",   "arrow_icon.png" } },
        { ConstructType::SHOOTER,    { ConstructType::SHOOTER,      10, "Devil",        "Sentient sentry, deviruchi devil.",     "devil_icon.png" } },
        { ConstructType::ICE,        { ConstructType::ICE,          10, "Ice",          "50% slow movement speed/attack speed.", "ice_icon.png" } },
        { ConstructType::CONFUSE,    { ConstructType::CONFUSE,      10, "Confuse",      "Makes you forget how your legs work.",   "confuse_icon.png" } },
        { ConstructType::REMOVE,     { ConstructType::REMOVE,       20, "Remove",       "Delet this.",                      "no_player.png" } },
    };
}