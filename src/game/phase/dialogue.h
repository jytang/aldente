#pragma once

#include "phase.h"
#include "ui/dialog_ui.h"

class DialoguePhase : public Phase {
public:
    DialoguePhase(Context &context, const proto::Phase &after, const std::string &title,
                      const std::vector<DialogUI::str_pair> &to_say, Scene *scene,
                      const std::function<void(Camera &)> &camera_setup);
    void s_setup() override;
    void s_teardown() override;
    void c_setup() override;
    void c_update() override;
    void c_teardown() override;
    std::string to_string() override;
    proto::Phase s_phase_when_done() override;

private:
    const std::string title;
    std::vector<DialogUI::str_pair> to_say;
    const proto::Phase after;
    Scene *scene;
    const std::function<void(Camera &)> camera_setup;
    int s_n_players_finished;
    boost::signals2::connection dialogue_conn;
};
