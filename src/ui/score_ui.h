#pragma once

#include "ui.h"
#include "ui_grid.h"
#include "ui_leaderboard_entry.h"

#include <vector>

class ScoreUI : public UI {
public:
    ScoreUI(float aspect);
    ~ScoreUI();
private:
    UIGrid score_grid;
    UIRectangle bg;
    std::vector<UILeaderboardEntry *> entries;
};