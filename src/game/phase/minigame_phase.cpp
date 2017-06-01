#include <game/game_state.h>
#include "minigame_phase.h"

MinigamePhase::MinigamePhase(Context& context) : TimedPhase(context) {
    minigames = {
        PenguinMG(context)
    };
    curr_mg = nullptr;
}

void MinigamePhase::s_setup() {
    transition_after(5, proto::Phase::BUILD);

    // Pick minigame and set up timer/connections
    // For now, just choose first one
    curr_mg = &(minigames[0]);

    //x.s_setup
    std::cerr << "[s] setup minigame\n";
    events::minigame::start_minigame_event();
}

void MinigamePhase::c_setup() {
    // TODO: client needs to know what minigame was chosen!!
    // For now, choose first one
    curr_mg = &(minigames[0]);

    //x.c_setup
    std::cerr << "[c] setup minigame\n";
    events::minigame::start_minigame_event();
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
    //bool finished = x.is_finished;

    /*if (all_players_done)
        return proto::Phase::BUILD;
    else*/
    return next;
}

void MinigamePhase::c_update() {
}

void MinigamePhase::s_teardown() {
    cancel_clock_every();
    // x.disconnect

    // Assigns rewards depending on player's ranking
    /*Player *curr_player;
    
    for (auto const &kv : goal_reached_flags) {
        curr_player = dynamic_cast<Player*>(GameObject::game_objects[kv.first]);
        assert(curr_player);
        curr_player->s_modify_stats([&, kv](PlayerStats &stats) {
            stats.add_coins(rewards[kv.second]);
        });
    }*/

    events::minigame::end_minigame_event();
}

void MinigamePhase::c_teardown() {
    // x. disconnect

    events::minigame::end_minigame_event();
}