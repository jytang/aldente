#pragma once

#include "ui.h"
#include "ui_text_box.h"
#include "ui_image_node.h"
#include "ui_grid.h"
#include "util/color.h"

#include <map>
#include <vector>
#include <string>

// Entry for a player on the leaderboard: ranking, portrait and amount of gold
class LeaderboardEntry : public UIContainer {
public:
    LeaderboardEntry(float start_x, float start_y,
                     float width, float height,
                     int ranking, GLuint portrait, int gold);
    void set_gold(int gold);
    int get_gold() { return gold; }
    void set_portrait(GLuint portrait);
    void set_ranking(int ranking);
private:
    UIRectangle bg;
    UIImageNode ranking_image;
    UIImageNode portrait_image;
    UITextBox gold_amount;
    UIImageNode gold_image;

    int gold;

    static map<int, Color> ranking_to_color; // static mapping from {1,2,3,4} to colors
    static map<int, std::string> ranking_to_image; // {1,2,3,4} to their images
};

class LeaderboardUI : public UI {
public:
    LeaderboardUI(float aspect);
    ~LeaderboardUI();
private:
    void sort_leaderboard(); // helper function to sort the entries by gold amount

    UIGrid leaderboard_grid;
    std::map<int, LeaderboardEntry *> ranking_to_entry; // maps rank to an entry
    std::map<int, LeaderboardEntry *> id_to_entry; // maps player ids to leaderboard entries
    static std::map<std::string, std::string> model_to_portrait; // maps model names to respective 2D portraits
    int next_entry_idx = 0;
};