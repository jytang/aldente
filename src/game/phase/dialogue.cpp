#include <game/game_state.h>
#include "dialogue.h"

DialoguePhase::DialoguePhase(Context &context, const proto::Phase &after, const std::string &title,
                             const std::vector<DialogUI::str_pair> &to_say, Scene *scene,
                             const std::function<void(Camera &)> &camera_setup)
    : Phase(context)
    , title(title)
    , to_say(to_say)
    , after(after)
    , scene(scene)
    , camera_setup(camera_setup)
    , s_n_players_finished(0) {}

void DialoguePhase::s_setup() {
    GameState::set_scene(scene);
    s_n_players_finished = 0;
    events::dialogue::s_player_finished_dialogue.connect([this](int id) {
        s_n_players_finished++;
        if (s_n_players_finished >= context.player_ids.size())
            next = after;
    });
}

void DialoguePhase::s_teardown() {
}

void DialoguePhase::c_setup() {
    GameState::set_scene(scene);

    // Set up the camera
    Camera *cam = &(scene->info.camera);
    cam->reset();
    camera_setup(*cam);
    cam->recalculate();

    events::ui::show_dialog(to_say, [this]() {
        proto::ClientMessage m;
        m.set_player_finished_dialogue(context.player_id);
        events::client::send(m);

        events::ui::show_notification("Waiting for other players...");
    });
}

void DialoguePhase::c_update() {
}

void DialoguePhase::c_teardown() {
}

std::string DialoguePhase::to_string() {
    return title;
}
