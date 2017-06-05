#include "ui_manager.h"

#include "events.h"
#include "game/construct_types.h"
#include "build_ui.h"
#include "clock_ui.h"
#include "gold_ui.h"
#include "debug_ui.h"
#include "leaderboard_ui.h"
#include "legend_ui.h"
#include "dialog_ui.h"
#include "main_menu_ui.h"
#include "score_ui.h"
#include "notification_ui.h"
#include "transition_ui.h"

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
    for (int i = 1; i <= 12; i++) {
        int num_available_constructs = Constructs::CONSTRUCTS.size();
        if (i < num_available_constructs)
            constructs.push_back(Constructs::CONSTRUCTS.at((ConstructType) i));
        else
            constructs.push_back(Constructs::CONSTRUCTS.at(ConstructType::REMOVE));
    }
    ui_map["build"] = new BuildUI(3, 4, aspect, constructs);

    /* CLOCK UI */
    ui_map["clock"] = new ClockUI(aspect);

    /* GOLD UI */
    ui_map["gold"] = new GoldUI(aspect);

    /* DEBUG UI */
    ui_map["debug"] = new DebugUI(aspect);

    /* LEADERBOARD UI */
    ui_map["leaderboard"] = new LeaderboardUI(aspect);

    /* LEGEND UI */
    ui_map["legend"] = new LegendUI(aspect, 15.f, 5.f, 1.f);

    /* DIALOG UI (should have high z-index) */
    ui_map["z9-dialog"] = new DialogUI(aspect, 80.f, 30.f, 2.f);

    ui_map["z5-notifs"] = new NotificationUI(aspect, 30.f, 15.f);

    /* MAIN MENU UI */
    ui_map["main-menu"] = new MainMenuUI(aspect);

    /* SCORE SCREEN UI */
    ui_map["score-screen"] = new ScoreUI(aspect);

    /* TRANSITION UI (highest z-index) */
    ui_map["z999-transition"] = new TransitionUI(aspect);
}

void UIManager::setup_listeners() {
    events::debug::toggle_ui_event.connect([&](void) {
        all_enabled = !all_enabled;
    });
}

void UIManager::draw() {
    if (!all_enabled) return; // don't draw anything if ui is completely disabled
    for (auto it = ui_map.begin(); it != ui_map.end(); ++it) {
        it->second->draw(renderer_2d);
    }
}
