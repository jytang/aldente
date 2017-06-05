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
        UIUnstretchedTextBox *delta_entry =
                new UIUnstretchedTextBox(3.f, 4.f,
                                         41.f * aspect, 0.f,
                                         30.f * aspect, 70.f / LEADERBOARD_ENTRIES, 0,
                                         UIUnstretchedTextBox::START, UIUnstretchedTextBox::END,
                                         Color::GOLD, Color::BLACK, 0.f);
        delta_entry->set_alpha(0.f); // nothing shows by default
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
        populate_scores();
        enable();

        // Do gold delta sequence.
        animate_deltas();
    });

    events::ui::disable_scoreboard.connect([&]() {
        disable_animated();
    });
}

void ScoreUI::disable_animated() {
    for (int i = 0; i < entries.size(); ++i) {
        entries[i]->animate_alpha(0.f, 0.5f);
    }
    // have bg disable the ui after it disappears
    bg.animate_alpha(0.f, 0.51f, [&]() {
        disable();
    });
}

void ScoreUI::animate_add_scores() {
    // Animate out scores, populate, then animate in.
    for (int i = 0; i < entries.size(); ++i) {
        // Have the first entry callback populate_scores
        if (i == 0) {
            entries[i]->animate_alpha(0.f, 0.5f, [&, i]() {
                populate_scores();
                entries[i]->animate_alpha(1.f, 0.5f);
            });
        } else {
            entries[i]->animate_alpha(0.f, 0.5f, [&, i]() {
                entries[i]->animate_alpha(1.f, 0.5f);
            });
        }
    }
}

void ScoreUI::animate_deltas() {
    int rank = 0;
    for (auto it = sorted_data.begin(); it != sorted_data.end(); ++it) {
        if (rank < deltas.size()) {
            int gold_delta = std::get<2>(*it);
            // display
            deltas[rank]->set_text((gold_delta > 0 ? "+" : "") + std::to_string(gold_delta));
            // add to gold amount
            std::get<1>(*it) = std::get<1>(*it) + gold_delta;

            // animate alpha channel
            // 3 nested lambdas, pretty good
            deltas[rank]->animate_alpha(1.f, 0.5f, [&, rank]() {
                Timer::get()->do_after(std::chrono::milliseconds(1000), [&, rank]() {
                    deltas[rank]->animate_alpha(0.f, 0.5f, [&](){
                        animate_add_scores();
                    });
                });
            });
        }
        rank++;
    }
}

void ScoreUI::populate_scores() {
    std::sort(sorted_data.begin(), sorted_data.end(), comparison_func);

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
