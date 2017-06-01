#include <game/game_state.h>
#include "minigame_phase.h"

MinigamePhase::MinigamePhase(Context& context) : TimedPhase(context) {
    minigames = {
        new PenguinMG(context)
    };
    curr_mg = nullptr;
}

MinigamePhase::~MinigamePhase() {
    for (auto & mg : minigames) {
        delete mg;
    }
}


void MinigamePhase::s_setup() {
    // Pick minigame and set up timer/connections
    // For now, just choose first one
    curr_mg = minigames[0];

    transition_after(curr_mg->get_time().count(), proto::Phase::BUILD);

    curr_mg->s_setup();
    events::minigame::start_minigame_event();
}

void MinigamePhase::c_setup() {
    // TODO: client needs to know what minigame was chosen!!
    // For now, choose first one
    curr_mg = minigames[0];

    curr_mg->c_setup();
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
    bool finished = curr_mg->is_finished();

    if (finished)
        return proto::Phase::BUILD;
    else
        return next;
}

void MinigamePhase::c_update() {
}

void MinigamePhase::s_teardown() {
    cancel_clock_every();
    curr_mg->s_teardown();

    /*
    // Assigns rewards depending on player's ranking
    Player *curr_player;
    
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
    curr_mg->c_teardown();

    events::minigame::end_minigame_event();
}