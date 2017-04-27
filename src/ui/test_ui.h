#pragma once

#include "ui.h"

#include "ui_grid.h"
#include "ui_rectangle.h"
#include "ui_text_node.h"

class TestUI : public UI {
public:
    TestUI(int num_cols, int num_rows, float aspect);
private:
    UIGrid ui_grid;
    UIRectangle rect, bottom_rect;
    UIContainer info_panel;
    UIRectangle info_rect;
    UITextNode title_label;
};
