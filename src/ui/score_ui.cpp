#include "score_ui.h"

#include "events.h"
#include "asset_loader.h"

#include <algorithm> // std::sort

#define LEADERBOARD_ENTRIES 4

// For score sorting purposes
bool comparison_func(std::pair<std::string, int> i, std::pair<std::string, int> j) {
    return i.second > j.second;
}

ScoreUI::~ScoreUI() {
    for (UILeaderboardEntry *entry : entries)
        delete entry;
}

ScoreUI::ScoreUI(float aspect)
    : UI(0.f, 0.f),
      bg(0.f, 0.f,
         100.f * aspect, 100.f,
         Color::BLACK, 0.7f),
      score_grid(30.f * aspect, 10.f, // starting coords
                 40.f * aspect, 70.f, // width and height
                 LEADERBOARD_ENTRIES, // num elts
                 1,                   // num cols
                 40.f * aspect,       // elt width
                 70.f / LEADERBOARD_ENTRIES, // elt height
                 Color::BLACK,               // bg color
                 1.f,   // padding
                 0.f,   // no selection halo
                 0.f) { // transparent bg

    attach(bg);
    attach(score_grid);

    for (int i = 0; i < LEADERBOARD_ENTRIES; ++i) {
        UILeaderboardEntry *entry =
                new UILeaderboardEntry(0, 0, // starting coords
                                       40.f * aspect, 70.f / LEADERBOARD_ENTRIES, // dimensions
                                       i, // ranking
                                       AssetLoader::get_texture("no_player.png"), 0);
        entries.push_back(entry);
        score_grid.attach_at(i, 0, *entry);
    }

    // disabled by default
    disable();

    /* EVENT LISTENERS */
    events::ui::enable_scoreboard.connect([&](const auto &data) {
        std::vector<std::pair<std::string, int>> sorted_data = data;
        std::sort(sorted_data.begin(), sorted_data.end(), comparison_func);

        int rank = 0;
        for (auto it = sorted_data.begin(); it != sorted_data.end(); ++it) {
            std::string model_name;
            int gold;
            std::tie(model_name, gold) = *it;

            if (rank < entries.size()) {  // minor validation
                entries[rank]->set_ranking(rank);
                entries[rank]->set_portrait(model_name);
                entries[rank]->set_gold(gold);
            }
            rank++;
        }

        // finally, enable the ui
        enable();
    });

    events::ui::disable_scoreboard.connect([&]() {
        disable();
    });
}