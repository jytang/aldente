#include <game/game_state.h>
#include "minigame_phase.h"
#include "input/modal_input.h"

std::string MinigamePhase::to_string() {
    return "MINIGAME PHASE";
}

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

    do_update = false;
    transition_after(6, curr_mg->get_time().count(), proto::Phase::BUILD);
    Timer::get()->do_after(std::chrono::seconds(6), [this]() {
        do_update = true;
    });

    curr_mg->s_setup();
    events::minigame::start_minigame_event();
}

void MinigamePhase::c_setup() {
    input::ModalInput::get()->set_mode(input::ModalInput::DISABLE);
    // TODO: client needs to know what minigame was chosen!!
    // For now, choose first one
    curr_mg = minigames[0];

    curr_mg->c_setup();
    events::ui::show_countdown({"3", "2", "1", "GO"}, []() {
        input::ModalInput::get()->set_mode(input::ModalInput::NORMAL);
        events::minigame::start_minigame_event();
    });
}

proto::Phase MinigamePhase::s_update() {
    if (!do_update) return proto::Phase::NOOP;

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

    events::minigame::end_minigame_event();
}

void MinigamePhase::c_teardown() {
    curr_mg->c_teardown();

    events::minigame::end_minigame_event();
}