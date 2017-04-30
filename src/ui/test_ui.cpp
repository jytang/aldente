#include "test_ui.h"

#include "util/colors.h"
#include "events.h"
#include "asset_loader.h"

TestUI::TestUI(int num_cols, int num_rows, float aspect, std::vector<ConstructData>& constructs)
    : UI(), // explicit call base class dflt constructor
      constructs(constructs),
      ui_grid(0, 0, 30.f * aspect, 80.f, num_cols*num_rows, num_cols, 12, 12, color::loz_green, 3, 1.f),
      rect(0, 0, 12, 12, color::loz_light_green),
      bottom_rect(0, 0, 12, 3, color::black),
      info_panel(0, 80.f),
      info_rect(0, 0, 30.f * aspect, 20.f, color::loz_dark_green),
      title_label("Select a block...", 2, 14, 1.f, 1.f, color::white),
      description_label("", 2, 8, 0.6f, 0.6f, color::white) {
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            ui_grid.attach_at(i, j, rect);

            ConstructData cd = constructs[i * num_cols + j];
            UIContainer* cont = new UIContainer(0, 0);
            UITextNode* text = new UITextNode(cd.name, 0, 0, 0.6f, 0.6f, color::white);
            UIImageNode* image = new UIImageNode(1, 1, 10, 10, color::white, AssetLoader::asset_loader->get_texture(cd.image));
            cont->attach(bottom_rect);
            cont->attach(*text);
            cont->attach(*image);
            ui_grid.attach_at(i, j, *cont);
        }
    }

    attach(ui_grid);

    info_panel.attach(info_rect);
    info_panel.attach(title_label);
    info_panel.attach(description_label);
    attach(info_panel);

    // Display info of first element by default
    update_info_panel(0);

    events::ui_grid_selection_event.connect([&](int content_index) {
        update_info_panel(content_index);
        events::build::construct_changed_event(constructs[content_index].type);
    });

    events::toggle_ui_event.connect([&](void) {
        if (enabled)
            disable();
        else
            enable();
    });
}

void TestUI::update_info_panel(int content_index) {
    title_label.set_text(constructs[content_index].name);
    description_label.set_text(constructs[content_index].description);
}
