#include "ui_leaderboard_entry.h"

#include "asset_loader.h"

std::map<std::string, std::string> UILeaderboardEntry::model_to_portrait = {
        {"boy_two", "dio.jpg"},
        {"lizard", "grass.png"},
        {"cat", "test.png"},
        {"tomato", "Tomato.jpg"}};
std::map<int, Color> UILeaderboardEntry::ranking_to_color = {
        {0, Color::GOLD},
        {1, Color::SILVER},
        {2, Color::BRONZE},
        {3, Color::INDIAN_RED}};
std::map<int, std::string> UILeaderboardEntry::ranking_to_image = {
        {0, "1st.png"},
        {1, "2nd.png"},
        {2, "3rd.png"},
        {3, "4th.png"}};

UILeaderboardEntry::UILeaderboardEntry(float start_x, float start_y,
                                   float width, float height,
                                   int ranking, GLuint portrait, int gold)
        : UIContainer(start_x, start_y),
          gold(gold),
          bg(start_x, start_y, width, height, ranking_to_color[ranking], 1.f),
          ranking_image(0, 0.1f * height, // starting coordinates
                        0.8f * height, 0.8f * height,
                        AssetLoader::get_texture(ranking_to_image[ranking])),
          portrait_image(0.8f * height, 0.1f * height,
                         0.8f * height, 0.8f * height,
                         portrait),
          gold_amount("0",
                      glm::max(1.6f * height, width - 1.2f * height),
                      0.25f * height,
                      0.8f * height, 0.5f * height,
                      Color::WHITE),
          gold_image(glm::max(1.6f * height, width - 1.2f * height) + 0.8f * height,
                     0.3f * height, // vertically centered
                     0.4f * height, 0.4f * height,
                     AssetLoader::get_texture("essence.png")) {

    attach(bg);

    attach(ranking_image);
    attach(portrait_image);
    attach(gold_amount);
    attach(gold_image);
}

void UILeaderboardEntry::set_gold(int gold) {
    this->gold = gold;
    gold_amount.set_text(std::to_string(gold));
}

void UILeaderboardEntry::set_portrait(std::string model) {
    portrait_image.set_image(AssetLoader::get_texture(model_to_portrait[model]));
}

void UILeaderboardEntry::set_ranking(int ranking) {
    ranking_image.set_image(AssetLoader::get_texture(ranking_to_image[ranking]));
    bg.set_color(ranking_to_color[ranking]);
}