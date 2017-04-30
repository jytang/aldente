#include "test_ui.h"

#include "util/colors.h"
#include "events.h"
#include "asset_loader.h"

TestUI::TestUI(int num_cols, int num_rows, float aspect)
    : UI(), // explicit call base class dflt constructor
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
        }
    }

    UIContainer* cont;
    UITextNode* text;
    UIImageNode* image;

    cont = new UIContainer(0, 0);
    text = new UITextNode("Chest", 0, 0, 0.6f, 0.6f, color::white);
    image = new UIImageNode(1, 1, 10, 10, color::white, AssetLoader::asset_loader->get_texture("test.png"));
    //cont->attach(bottom_rect);
    //cont->attach(*text);
    cont->attach(*image);
    ui_grid.attach_at(0, 0, *cont);

    cont = new UIContainer(0, 0);
    text = new UITextNode("Remove", 0, 0, 0.6f, 0.6f, color::white);
    image = new UIImageNode(1, 1, 10, 10, color::white, AssetLoader::asset_loader->get_texture("Tomato.jpg"));
    //cont->attach(bottom_rect);
    //cont->attach(*text);
    cont->attach(*image);
    ui_grid.attach_at(0, 1, *cont);

    attach(ui_grid);

    info_panel.attach(info_rect);
    info_panel.attach(title_label);
    info_panel.attach(description_label);
    attach(info_panel);

    events::build::construct_changed_event.connect([&](ConstructType c) {
        std::string title_text, desc_text;
        switch (c) {
            case CHEST:
                title_text = "Chest";
                desc_text = "This is a chest.";
                break;
            case REMOVE:
                title_text = "Delete";
                desc_text = "This is a deletion.";
                break;
            default:
                title_text = "Select a block...";
                desc_text = "";
                break;
        }
        title_label.set_text(title_text);
        description_label.set_text(desc_text);
    });

    events::toggle_ui_event.connect([&](void) {
        if (enabled)
            disable();
        else
            enable();
    });
}
