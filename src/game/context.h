#pragma once

#include "game_objects/game_object.h"
#include "game_objects/player.h"
#include <set>

// Forward-declare
class Player;

class Context {
public:
    // Common
    std::vector<int> player_ids;

    // Server
    std::set<GameObject*> updated_objects;
    std::set<std::pair<int, int>> collisions;
    std::set<std::pair<int, int>> interacts;
    std::unordered_map<int, bool> ready_flags; // <Player id, if ready>

    // Client
    Player* client_player;
    int player_id;
    bool player_finished;
};