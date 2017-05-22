#pragma once

#include "ui.h"
#include "ui_text_box.h"
#include "ui_image_node.h"

class GoldUI : public UI {
public:
    GoldUI(float aspect);
private:
    void set_gold(int gold);

    UITextBox gold_delta;
    UITextBox total_gold;
    UIImageNode gold_image;
};