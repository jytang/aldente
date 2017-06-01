#include "score_ui.h"

#include "events.h"
#include "asset_loader.h"
#include "timer.h"

#include <algorithm> // std::sort

#define LEADERBOARD_ENTRIES 4

// For score sorting purposes
bool comparison_func(std::tuple<std::string, int, int> i, std::tuple<std::string, int, int> j) {
    return std::get<1>(i) > std::get<1>(j);
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
        UITextBox *delta_entry =
                new UITextBox("+0", 40.f * aspect, 0.25f * 70.f / LEADERBOARD_ENTRIES,
                              5.f * aspect, 5.f, Color::WHITE);
        delta_entry->text_node.set_alpha(1.f); // nothing shows by default
        entries.push_back(entry);
        deltas.push_back(delta_entry);
        score_grid.attach_at(i, 0, *entry);
        score_grid.attach_at(i, 0, *delta_entry);
    }

    // disabled by default
    disable();

    /* EVENT LISTENERS */
    events::ui::scoreboard_sequence.connect([&](const auto &data) {
        sorted_data = data;
        std::sort(sorted_data.begin(), sorted_data.end(), comparison_func);

        populate_scores();
        enable();

        // Do gold delta sequence.
        animate_deltas();
    });

    events::ui::disable_scoreboard.connect([&]() {
        disable();
    });
}

void ScoreUI::animate_deltas() {
    int rank = 0;
    for (auto it = sorted_data.begin(); it != sorted_data.end(); ++it) {
        if (rank < deltas.size()) {
            int gold_delta = std::get<2>(*it);
            deltas[rank]->set_text("+" + std::to_string(gold_delta));

            // animate alpha channel
            /*deltas[rank]->text_node.animate_alpha(1.f, 1.f); /*, [&, rank]() {
                Timer::get()->do_after(std::chrono::milliseconds(500), [&, rank]() {
                    deltas[rank]->text_node.animate_alpha(0.f, 1.f);
                });
            }); */
        }
        rank++;
    }
}

void ScoreUI::populate_scores() {
    int rank = 0;
    for (auto it = sorted_data.begin(); it != sorted_data.end(); ++it) {
        std::string model_name;
        int gold;
        int gold_delta;
        std::tie(model_name, gold, gold_delta) = *it;

        if (rank < entries.size()) {  // minor validation
            entries[rank]->set_ranking(rank);
            entries[rank]->set_portrait(model_name);
            entries[rank]->set_gold(gold);
        }
        rank++;
    }
}
