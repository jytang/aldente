#include "ui_manager.h"

#include "events.h"
#include "game/construct_types.h"
#include "build_ui.h"
#include "clock_ui.h"
#include "gold_ui.h"
#include "fps_ui.h"
#include "leaderboard_ui.h"

UIManager::~UIManager() {
    for (auto it = ui_map.begin(); it != ui_map.end(); ++it) {
        delete it->second;
    }
}

UIManager::UIManager(float aspect)
        : aspect(aspect) {
    setup_uis();
    setup_listeners();
}

void UIManager::setup_uis() {
    /* BUILD UI */
    std::vector<ConstructData> constructs;
    for (int i = 0; i < 12; i++) {
        int type = (i % 3) + 1;
        switch (type) {
        case ConstructType::CHEST:
            constructs.push_back(Constructs::CONSTRUCTS.at(ConstructType::CHEST));
            break;
        case ConstructType::SPIKES:
            constructs.push_back(Constructs::CONSTRUCTS.at(ConstructType::SPIKES));
            break;
        case ConstructType::REMOVE:
            constructs.push_back(Constructs::CONSTRUCTS.at(ConstructType::REMOVE));
            break;
        default:
            break;
        }
    }
    ui_map["build"] = new BuildUI(3, 4, aspect, constructs);

    /* CLOCK UI */
    ui_map["clock"] = new ClockUI(aspect);

    /* GOLD UI */
    ui_map["gold"] = new GoldUI(aspect);

    /* FPS UI */
    ui_map["fps"] = new FPSUI(aspect);

    /* LEADERBOARD UI */
    ui_map["leaderboard"] = new LeaderboardUI(aspect);
}

void UIManager::setup_listeners() {
    events::debug::toggle_ui_event.connect([&](void) {
        all_enabled = !all_enabled;
    });
}

void UIManager::draw() {
    if (!all_enabled) return; // don't draw anything if ui is completely disabled
    for (auto it = ui_map.begin(); it != ui_map.end(); ++it) {
        it->second->draw();
    }
}