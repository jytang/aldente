#pragma once

#include "ui.h"
#include "ui_text_box.h"
#include "ui_image_node.h"
#include "ui_unstretched_text_box.h"

// Deltas support up to 4 digits.
class GoldUI : public UI {
public:
    GoldUI(float aspect);
private:
    int cur_gold;
    int cur_delta;
    bool delta_showing;
    std::function<void()> cancel_delta_out;

    void set_gold(int gold);
    void fade_delta_after(int ms);

    UIRectangle bg;
    UIUnstretchedTextBox gold_delta;
    UITextBox total_gold;
    UIImageNode gold_image;
};