#include <game/game_state.h>
#include <dialogues.h>
#include <audio/audio_manager.h>
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

    events::ui::show_dialog(dialogue::END, [this, winner]() {
        events::sound_effects_event(events::AudioData(AudioManager::FANFARE_SOUND, false));
        // Fade end title after 3 second delay.
        Timer::get()->do_after(std::chrono::milliseconds(3000), []() {
            events::ui::fade_end_title();
        });

        GameState::end_scene.cancel_cycle();
        GameState::end_scene.get_placeholder()->c_setup_player_model(winner->c_get_model_index());

        if (winner->c_get_model_index() == 0 || winner->c_get_model_index() == 3)
            GameState::end_scene.get_placeholder()->emote(true, 0.7f);
        else
            GameState::end_scene.get_placeholder()->emote(true);
    });

    events::end::start_end_event();
}

void EndPhase::c_update() {
}

void EndPhase::c_teardown() {
}

std::string EndPhase::to_string() {
    return "AWAKENING";
}

proto::Phase EndPhase::s_phase_when_done() {
    return proto::Phase::MENU;
}
