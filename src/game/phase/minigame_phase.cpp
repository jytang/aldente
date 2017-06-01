#include <game/game_state.h>
#include "minigame_phase.h"

MinigamePhase(Context& context) : TimedPhase(context) {
}

void MinigamePhase::s_setup() {
    // Pick minigame and set up timer/connections
    //transition_after(X, proto::Phase::BUILD);
    //x.connect_listeners
    
}

void MinigamePhase::c_setup() {
    //x.connect_listeners
}

proto::Phase MinigamePhase::s_update() {
    GameState::physics.update();

    // Send the position and orientation of the specified game objects.
    // Currently sending all Player objects and Constructs.
    for (auto const & o : GameObject::game_objects) {
        if (!dynamic_cast<Tile*>(o.second) && !dynamic_cast<Grid*>(o.second))
            context.updated_objects.insert(o.second);
    }
    events::dungeon::update_state_event(&context);

    // Ask minigame if early termination is needed
    /*bool finished = x.is_finished;

    if (all_players_done)
        return proto::Phase::BUILD;
    else*/
    return next;
}

void MinigamePhase::c_update() {
}

void MinigamePhase::s_teardown() {
    cancel_clock_every();
    // x.disconnect

    /*// Assigns rewards depending on player's ranking
    Player *curr_player;
    for (auto const &kv : goal_reached_flags) {
        curr_player = dynamic_cast<Player*>(GameObject::game_objects[kv.first]);
        assert(curr_player);
        curr_player->s_modify_stats([&, kv](PlayerStats &stats) {
            stats.add_coins(rewards[kv.second]);
        });
    }*/
}

void MinigamePhase::c_teardown() {
    // x. disconnect
}