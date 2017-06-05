#pragma once

#include "ui.h"
#include "ui_text_box.h"
#include "ui_image_node.h"
#include "ui_grid.h"
#include "util/color.h"
#include "asset_loader.h"
#include "ui_leaderboard_entry.h"

#include <map>
#include <vector>
#include <string>

class LeaderboardUI : public UI {
public:
    LeaderboardUI(float aspect);
    ~LeaderboardUI();
    void enable_animated() override;
    void disable_animated() override;
private:
    void sort_leaderboard(); // helper function to sort the entries by gold amount

    float aspect;
    UIGrid leaderboard_grid;
    std::map<int, UILeaderboardEntry *> ranking_to_entry; // maps rank to an entry
    std::map<int, UILeaderboardEntry *> id_to_entry; // maps player ids to leaderboard entries
    int next_entry_idx = 0;
};