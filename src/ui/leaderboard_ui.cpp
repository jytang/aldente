#include "leaderboard_ui.h"

#include "util/color.h"
#include "events.h"

#define LEADERBOARD_ENTRIES 4

std::map<std::string, std::string> LeaderboardUI::model_to_portrait = {
        {"boy_two", "dio.jpg"},
        {"lizard", "grass.png"},
        {"cat", "test.png"},
        {"tomato", "Tomato.jpg"}};

std::map<int, Color> LeaderboardEntry::ranking_to_color = {
        {0, Color::GOLD},
        {1, Color::SILVER},
        {2, Color::BRONZE},
        {3, Color::INDIAN_RED}};
std::map<int, std::string> LeaderboardEntry::ranking_to_image = {
        {0, "cobblestone.png"},
        {1, "wall.png"},
        {2, "cloudwall.png"},
        {3, "grass.png"}};

LeaderboardUI::~LeaderboardUI() {
    for (auto &pair : ranking_to_entry)
        delete pair.second;
}

LeaderboardUI::LeaderboardUI(float aspect)
    : UI(0.f, 20.f),
      leaderboard_grid(0, 0,
                       24.f * aspect, 60.f,
                       LEADERBOARD_ENTRIES, // num elements
                       1, // num columns
                       24.f * aspect, // element width
                       60.f / LEADERBOARD_ENTRIES, // element height
                       Color::BLACK, // bg color
                       0.f, // padding between each element
                       0.f, // no selection halo
                       0.5f) {

    attach(leaderboard_grid);

    for (int i = 0; i < LEADERBOARD_ENTRIES; ++i) {
        LeaderboardEntry *entry = new LeaderboardEntry(0, 0, // starting coords
                                                       24.f * aspect, 60.f / LEADERBOARD_ENTRIES, // dimensions
                                                       i, // ranking
                                                       AssetLoader::get_texture("dio.jpg"), 0);
        ranking_to_entry[i] = entry;
        leaderboard_grid.attach_at(i, 0, *entry);
    }

    // disabled by default
    disable();

    /* EVENT LISTENERS */
    events::ui::disable_leaderboard.connect([&]() {
        disable();
    });
    events::ui::enable_leaderboard.connect([&]() {
        enable();
    });
    events::ui::toggle_leaderboard.connect([&](){
        toggle();
    });
    // Hide when build phase begins.
    events::build::start_build_event.connect([&]() {
        disable();
    });

    events::ui::leaderboard_update.connect([&](int player_id, int gold, std::string model) {
        // Set up association from player id to leaderboard entry if none exists.
        auto it = id_to_entry.find(player_id);
        if (it == id_to_entry.end()) {
            id_to_entry[player_id] = ranking_to_entry[next_entry_idx];
            next_entry_idx++;
        }

        // Update gold amount and reflow leaderboard ui.
        id_to_entry[player_id]->set_gold(gold);
        GLuint portrait_id = AssetLoader::get_texture(model_to_portrait[model]);
        id_to_entry[player_id]->set_portrait(portrait_id);
        sort_leaderboard();
    });
}

void LeaderboardUI::sort_leaderboard() {
    for (int i = 0; i < LEADERBOARD_ENTRIES; ++i) {
        // Detach this ranking entry.
        leaderboard_grid.detach_at(i, 0, *ranking_to_entry[i]);

        // TODO: Deal with ties
        // Use inefficient sorting because LEADERBOARD_ENTRIES is small.
        LeaderboardEntry *best = ranking_to_entry[i];
        int best_index = i;
        for (int j = i+1; j < LEADERBOARD_ENTRIES; ++j) {
            if (ranking_to_entry[j]->get_gold() > best->get_gold()) {
                best = ranking_to_entry[j];
                best_index = j;
            }
        }

        // Swap if necessary.
        if (best_index != i) {
            LeaderboardEntry *tmp = ranking_to_entry[i];
            ranking_to_entry[i] = ranking_to_entry[best_index];
            ranking_to_entry[best_index] = tmp;
        }

        // Update ranking visuals
        ranking_to_entry[i]->set_ranking(i);

        leaderboard_grid.attach_at(i, 0, *ranking_to_entry[i]);
    }
}

LeaderboardEntry::LeaderboardEntry(float start_x, float start_y,
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
    this->gold = gold;
    gold_amount.set_text(std::to_string(gold));
}

void LeaderboardEntry::set_portrait(GLuint portrait) {
    portrait_image.set_image(portrait);
}

void LeaderboardEntry::set_ranking(int ranking) {
    ranking_image.set_image(AssetLoader::get_texture(ranking_to_image[ranking]));
    bg.set_color(ranking_to_color[ranking]);
}