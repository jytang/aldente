#include <util/config.h>
#include "minigame_results.h"

MinigameResultsPhase::MinigameResultsPhase(Context &context)
        : TimedPhase(context) {}

void MinigameResultsPhase::s_setup() {
    Config::config->get_value(Config::str_num_rounds, n_rounds);

    transition_after(0, 10, s_phase_when_done());

    // Do any extra scene stuff here
}

void MinigameResultsPhase::s_teardown() {
}

void MinigameResultsPhase::c_setup() {
    Config::config->get_value(Config::str_num_rounds, n_rounds);

    std::vector<std::tuple<std::string, int, int>> results;

    for (int player_id : context.player_ids) {
        Player* player = dynamic_cast<Player*>(GameObject::game_objects[player_id]);
        std::string model_name = Player::PLAYER_MODELS[player->c_get_model_index()];
        results.push_back({ model_name, context.pre_mg_gold[player_id], player->get_coins() - context.pre_mg_gold[player_id] });
    }

    events::ui::scoreboard_sequence(results);

    // Do any extra scene stuff here
}

void MinigameResultsPhase::c_update() {
}

void MinigameResultsPhase::c_teardown() {
    events::ui::disable_scoreboard();

    // Stop all music/sounds
    events::stop_music_event();
    events::stop_all_sounds_event();
}

std::string MinigameResultsPhase::to_string() {
    return "Minigame Results";
}

proto::Phase MinigameResultsPhase::s_phase_when_done() {
    return context.current_round == n_rounds ? proto::Phase::END : proto::Phase::BUILD;
}
