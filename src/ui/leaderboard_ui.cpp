#include "leaderboard_ui.h"

#include "events.h"

std::map<int, Color> LeaderboardEntry::ranking_to_color = {
        {1, Color::GOLD},
        {2, Color::SILVER},
        {3, Color::BRONZE},
        {4, Color::OLIVE_GREEN}};
std::map<int, GLuint> LeaderboardEntry::ranking_to_image; // lazily populated (need assets loaded first)

LeaderboardUI::LeaderboardUI(float aspect)
    : UI(0.f, 20.f),
      leaderboard_grid(0, 0,
                       30.f * aspect, 60.f,
                       4, // num elements
                       1, // num columns
                       30.f * aspect, // element width
                       15.f, // element height
                       Color::BLACK, // bg color
                       2.f, // padding between each element
                       0.f, // no selection halo
                       0.5f), // semi-transparent background
      entry1(0, 0, 30.f * aspect, 15.f, 1, AssetLoader::get_texture("dio.jpg"), 0),
      entry2(0, 0, 30.f * aspect, 15.f, 2, AssetLoader::get_texture("Tomato.jpg"), 0),
      entry3(0, 0, 30.f * aspect, 15.f, 3, AssetLoader::get_texture("test.png"), 0),
      entry4(0, 0, 30.f * aspect, 15.f, 4, AssetLoader::get_texture("cloudwall.png"), 0) {

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
}

LeaderboardEntry::LeaderboardEntry(float start_x, float start_y,
                                   float width, float height,
                                   int ranking, GLuint portrait, int gold)
    : UIContainer(start_x, start_y),
      bg(start_x, start_y, width, height, ranking_to_color[ranking], 0.5f),
      ranking_image(0, 0, // starting coordinates
                    width * 0.2f, height,
                    AssetLoader::get_texture("grass.png")),
      portrait_image(width * 0.2f, 0,
                     width * 0.3f, height,
                     portrait),
      gold_image(width * 0.8f, 0,
                 width * 0.2f, width * 0.2f,
                 AssetLoader::get_texture("essence.png")) {

    attach(bg);

    attach(ranking_image);
    attach(portrait_image);
    attach(gold_image);
}