#include "leaderboard_ui.h"

#include "events.h"

std::map<int, Color> LeaderboardEntry::ranking_to_color = {
        {1, Color::GOLD},
        {2, Color::SILVER},
        {3, Color::BRONZE},
        {4, Color::INDIAN_RED}};
std::map<int, GLuint> LeaderboardEntry::ranking_to_image; // lazily populated (need assets loaded first)

LeaderboardUI::LeaderboardUI(float aspect)
    : UI(0.f, 20.f),
      leaderboard_grid(0, 0,
                       24.f * aspect, 60.f,
                       4, // num elements
                       1, // num columns
                       24.f * aspect, // element width
                       15.f, // element height
                       Color::BLACK, // bg color
                       0.f, // padding between each element
                       0.f, // no selection halo
                       0.5f), // semi-transparent background
      entry1(0, 0, 24.f * aspect, 15.f, 1, AssetLoader::get_texture("dio.jpg"), 0),
      entry2(0, 0, 24.f * aspect, 15.f, 2, AssetLoader::get_texture("Tomato.jpg"), 0),
      entry3(0, 0, 24.f * aspect, 15.f, 3, AssetLoader::get_texture("test.png"), 0),
      entry4(0, 0, 24.f * aspect, 15.f, 4, AssetLoader::get_texture("cloudwall.png"), 0) {

    attach(leaderboard_grid);
    leaderboard_grid.attach_at(0, 0, entry1);
    leaderboard_grid.attach_at(1, 0, entry2);
    leaderboard_grid.attach_at(2, 0, entry3);
    leaderboard_grid.attach_at(3, 0, entry4);

    // disabled by default
    disable();

    // Setup listener to activate.
    events::ui::display_leaderboard.connect([&](){
        enable();
    });

    // Deactivate.
    events::ui::hide_leaderboard.connect([&](){
        disable();
    });

    events::c_player_stats_updated.connect([&](const proto::PlayerStats &update) {
        int coin_array[] = {update.coins(), update.coins(), update.coins(), update.coins()};
        populate_scores(coin_array);
    });
}

void LeaderboardUI::populate_scores(int scores[4]) {
    // Sort by gold amount. Change only gold and portrait displays.
    entry1.set_gold(scores[0]);
    entry2.set_gold(scores[1]);
    entry3.set_gold(scores[2]);
    entry4.set_gold(scores[3]);
}

LeaderboardEntry::LeaderboardEntry(float start_x, float start_y,
                                   float width, float height,
                                   int ranking, GLuint portrait, int gold)
    : UIContainer(start_x, start_y),
      bg(start_x, start_y, width, height, ranking_to_color[ranking], 1.f),
      ranking_image(0, 0.1f * height, // starting coordinates
                    0.8f * height, 0.8f * height,
                    AssetLoader::get_texture("grass.png")),
      portrait_image(0.8f * height, 0.1f * height,
                     0.8f * height, 0.8f * height,
                     portrait),
      gold_amount("100",
                  1.6f * height, 0.25f * height,
                  0.8f * height, 0.5f * height,
                  Color::WHITE),
      gold_image(2.4f * height, 0.3f * height, // vertically centered
                 0.4f * height, 0.4f * height,
                 AssetLoader::get_texture("essence.png")) {

    attach(bg);

    attach(ranking_image);
    attach(portrait_image);
    attach(gold_amount);
    attach(gold_image);
}

void LeaderboardEntry::set_gold(int gold) {
    gold_amount.set_text(std::to_string(gold));
}

void LeaderboardEntry::set_portrait(GLuint portrait) {
    portrait_image.set_image(portrait);
}