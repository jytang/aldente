#include "test_ui.h"

#include "util/colors.h"
#include "events.h"

TestUI::TestUI(int num_cols, int num_rows, float aspect)
    : UI(), // explicit call base class dflt constructor
      ui_grid(0, 0, 30.f * aspect, 80.f, num_cols*num_rows, num_cols, 9, 9, color::indian_red),
      rect(0, 0, 9, 9, color::windwaker_green),
      bottom_rect(0, 0, 9, 2, color::black),
      info_panel(0, 80.f),
      info_rect(0, 0, 30.f * aspect, 20.f, color::ocean_blue),
      title_label("Select a block", 0, 0, 1.f, 1.f, color::white) {
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            ui_grid.attach_at(i, j, rect);

            if (i == 0 && j == 0) {
                UIContainer* cont = new UIContainer(0, 0);
                UITextNode* text = new UITextNode("Chest", 0, 0, 0.4f, 0.4f, color::white);
                cont->attach(bottom_rect);
                cont->attach(*text);
                ui_grid.attach_at(i, j, *cont);
            }
            else if (i == 0 && j == 1) {
                UIContainer* cont = new UIContainer(0, 0);
                UITextNode* text = new UITextNode("Remove", 0, 0, 0.4f, 0.4f, color::white);
                cont->attach(bottom_rect);
                cont->attach(*text);
                ui_grid.attach_at(i, j, *cont);
            }
        }
    }
    attach(ui_grid);

    info_panel.attach(info_rect);
    info_panel.attach(title_label);
    attach(info_panel);

    events::build::construct_changed_event.connect([&](ConstructType c) {
        std::string text;
        switch (c) {
            case CHEST:
                text = "Building chest"; break;
            case REMOVE:
                text = "Removing"; break;
            default:
                text = "Select a block"; break;
        }
        title_label.set_text(text);
    });
}
