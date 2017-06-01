#pragma once

#include "ui_container.h"
#include "ui_rectangle.h"
#include "ui_image_node.h"
#include "ui_text_box.h"
#include "util/color.h"

#include <map>

// Entry for a player on the leaderboard: ranking, portrait and amount of gold
class UILeaderboardEntry : public UIContainer {
public:
    UILeaderboardEntry(float start_x, float start_y,
                     float width, float height,
                     int ranking, GLuint portrait, int gold);
    void set_gold(int gold);
    int get_gold() { return gold; }
    void set_portrait(std::string model);
    void set_ranking(int ranking);
private:
    UIRectangle bg;
    UIImageNode ranking_image;
    UIImageNode portrait_image;
    UITextBox gold_amount;
    UIImageNode gold_image;

    int gold;

    static std::map<int, Color> ranking_to_color; // static mapping from {1,2,3,4} to colors
    static std::map<int, std::string> ranking_to_image; // {1,2,3,4} to their images
    static std::map<std::string, std::string> model_to_portrait; // maps model names to respective 2D portraits
};
