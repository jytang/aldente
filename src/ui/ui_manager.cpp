#include "ui_manager.h"

#include "events.h"
#include "game/construct_types.h"
#include "build_ui.h"
#include "clock_ui.h"

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
        if (i % 2 == 0)
            constructs.push_back(Constructs::CHEST);
        else
            constructs.push_back(Constructs::REMOVE);
    }
    BuildUI *build_ui = new BuildUI(3, 4, aspect, constructs);
    ui_map["build"] = build_ui;

    /* CLOCK UI */
    ClockUI *clock_ui = new ClockUI(aspect);
    ui_map["clock"] = clock_ui;
}

void UIManager::setup_listeners() {
    events::debug::toggle_ui_event.connect([&](void) {
        all_enabled = !all_enabled;
    });

    // Build UI disable/enable triggers.
    // Enables Build UI on the start of the build phase
    events::build::start_build_event.connect([&]() {
        ui_map["build"]->enable(); // TODO: assert that ui_map["build"] exists?
    });

    // Disables Build UI at the end of the build phase
    events::build::end_build_event.connect([&]() {
        ui_map["build"]->disable();
    });
}

void UIManager::draw() {
    if (!all_enabled) return; // don't draw anything if ui is completely disabled
    for (auto it = ui_map.begin(); it != ui_map.end(); ++it) {
        it->second->draw();
    }
}