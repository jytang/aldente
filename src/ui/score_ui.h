#pragma once

#include "ui.h"
#include "ui_grid.h"
#include "ui_leaderboard_entry.h"
#include "ui_unstretched_text_box.h"

#include <vector>

class ScoreUI : public UI {
public:
    ScoreUI(float aspect);
    ~ScoreUI();
private:
    void populate_scores();
    void animate_deltas();
    void animate_add_scores();

    UIGrid score_grid;
    UIRectangle bg;
    std::vector<UILeaderboardEntry *> entries;
    std::vector<UIUnstretchedTextBox *> deltas;
    std::vector<std::tuple<std::string, int, int>> sorted_data;
};