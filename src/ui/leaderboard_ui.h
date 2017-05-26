#pragma once

#include "ui.h"
#include "ui_text_box.h"
#include "ui_image_node.h"
#include "ui_grid.h"
#include "util/color.h"

#include <map>

// Entry for a player on the leaderboard: ranking, portrait and amount of gold
class LeaderboardEntry : public UIContainer {
public:
    LeaderboardEntry(float start_x, float start_y,
                     float width, float height,
                     int ranking, GLuint portrait, int gold);
    void set_gold(int gold);
    void set_portrait(GLuint portrait);
private:
    UIRectangle bg;
    UIImageNode ranking_image;
    UIImageNode portrait_image;
    UITextBox gold_amount;
    UIImageNode gold_image;

    static map<int, Color> ranking_to_color; // static mapping from {1,2,3,4} to colors
    static map<int, GLuint> ranking_to_image; // {1,2,3,4} to their images
};

class LeaderboardUI : public UI {
public:
    LeaderboardUI(float aspect);

    void populate_scores(int scores[4]);
private:
    UIGrid leaderboard_grid;
    LeaderboardEntry entry1, entry2, entry3, entry4;
};