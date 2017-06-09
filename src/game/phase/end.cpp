#include "end.h"
#include "dialogues.h"

EndPhase::EndPhase(Context &context)
    : Phase(context) {}

void EndPhase::s_setup() {
    // Set the scene
    // GameState::set_scene(&GameState::end_scene);
}

void EndPhase::s_teardown() {
}

void EndPhase::c_setup() {
    // Set the scene
    GameState::set_scene(&GameState::end_scene);
    GameState::end_scene.reset_camera();

    int high_score = 0;
    Player* winner = nullptr;
    for (int player_id : context.player_ids) {
        Player* player = dynamic_cast<Player*>(GameObject::game_objects[player_id]);
        if (!winner || player->get_coins() > high_score) {
            winner = player;
            high_score = player->get_coins();
        }
    }

    // TODO: use the winner player (and their gold amount if you want)
    std::cerr << "winner player: " << winner->get_id() << " with gold: " << high_score << "\n";

    events::ui::show_dialog(dialogue::END, [this, winner]() {
        GameState::end_scene.cancel_cycle();
        GameState::end_scene.get_placeholder()->c_setup_player_model(winner->c_get_model_index());
    });
}

void EndPhase::c_update() {
}

void EndPhase::c_teardown() {
}

std::string EndPhase::to_string() {
    return "Awakening";
}

proto::Phase EndPhase::s_phase_when_done() {
    return proto::Phase::MENU;
}
