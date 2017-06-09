#include "leaderboard_ui.h"

#include "util/color.h"
#include "events.h"

#define LEADERBOARD_ENTRIES 4

LeaderboardUI::~LeaderboardUI() {
    for (auto &pair : ranking_to_entry)
        delete pair.second;
}

LeaderboardUI::LeaderboardUI(float aspect)
    : UI(0.f, 20.f),
      leaderboard_grid(-28.f * aspect, 0.f,
                       28.f * aspect, 60.f,
                       LEADERBOARD_ENTRIES, // num elements
                       1, // num columns
                       28.f * aspect, // element width
                       60.f / LEADERBOARD_ENTRIES, // element height
                       Color::BLACK, // bg color
                       0.f, // padding between each element
                       0.f, // no selection halo
                       0.5f),
      aspect(aspect) {

    attach(leaderboard_grid);

    for (int i = 0; i < LEADERBOARD_ENTRIES; ++i) {
        UILeaderboardEntry *entry = new UILeaderboardEntry(0, 0, // starting coords
                                                       28.f * aspect, 60.f / LEADERBOARD_ENTRIES, // dimensions
                                                       i, // ranking
                                                       AssetLoader::get_texture("no_player.png"), 0);
        ranking_to_entry[i] = entry;
        leaderboard_grid.attach_at(i, 0, *entry);
    }

    // disabled by default
    disable();

    /* EVENT LISTENERS */
    events::ui::enable_leaderboard.connect([&]() {
        enable_animated();
    });

    events::ui::disable_leaderboard.connect([&]() {
        disable_animated();
    });

    events::ui::toggle_leaderboard.connect([&](){
        if (enabled) {
            disable_animated();
        } else {
            enable_animated();
        }
    });
    // Hide when build phase begins.
    events::build::start_build_event.connect([&]() {
        disable_animated();
    });

    events::ui::leaderboard_update.connect([this](int player_id, int gold, std::string model) {
        // Set up association from player id to leaderboard entry if none exists.
        auto it = id_to_entry.find(player_id);
        if (it == id_to_entry.end()) {
            id_to_entry[player_id] = ranking_to_entry[next_entry_idx];
            next_entry_idx++;
        } else {
            1 + 1;
        }

        // Update gold amount and reflow leaderboard ui.
        id_to_entry[player_id]->set_gold(gold);
        id_to_entry[player_id]->set_portrait(model);
        sort_leaderboard();
    });
}

void LeaderboardUI::sort_leaderboard() {
    for (int i = 0; i < LEADERBOARD_ENTRIES; ++i) {
        // Detach this ranking entry.
        leaderboard_grid.detach_at(i, 0, *ranking_to_entry[i]);

        // TODO: Deal with ties
        // Use inefficient sorting because LEADERBOARD_ENTRIES is small.
        UILeaderboardEntry *best = ranking_to_entry[i];
        int best_index = i;
        for (int j = i+1; j < LEADERBOARD_ENTRIES; ++j) {
            if (ranking_to_entry[j]->get_gold() > best->get_gold()) {
                best = ranking_to_entry[j];
                best_index = j;
            }
        }

        // Swap if necessary.
        if (best_index != i) {
            UILeaderboardEntry *tmp = ranking_to_entry[i];
            ranking_to_entry[i] = ranking_to_entry[best_index];
            ranking_to_entry[best_index] = tmp;
        }

        // Update ranking visuals
        ranking_to_entry[i]->set_ranking(i);

        leaderboard_grid.attach_at(i, 0, *ranking_to_entry[i]);
    }
}

void LeaderboardUI::enable_animated() {
    enable();
    leaderboard_grid.set_alpha(0.f);
    leaderboard_grid.animate_alpha(1.f, 0.2f);
    leaderboard_grid.animate_to(0.f, 0.f, 0.1f);
}

void LeaderboardUI::disable_animated() {
    leaderboard_grid.animate_to(-28.f * aspect, 0.f, 0.1f);
    leaderboard_grid.animate_alpha(0.f, 0.2f, [&]() {
        disable();
    });
}