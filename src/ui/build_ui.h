#pragma once

#include "ui.h"

#include "ui_grid.h"
#include "ui_rectangle.h"
#include "ui_text_node.h"
#include "ui_image_node.h"
#include "game/construct_types.h"

#include "ui_clock.h"

class BuildUI : public UI {
public:
    BuildUI(int num_cols, int num_rows, float aspect, std::vector<ConstructData>& constructs);
    ~BuildUI();
private:
    // Displays the construct's (of the given index) info in the display panel.
    void update_info_panel(int content_index);

    std::vector<ConstructData> constructs;
    UIGrid ui_grid;
    UIRectangle rect;
    UIContainer info_panel, player_panel, shop_panel;
    UIRectangle info_rect, player_rect;
    UITextNode title_label, description_label, cost_label, balance_label;
    UIClock clock;

    std::vector<UIImageNode*> images;
};
