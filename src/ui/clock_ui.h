#pragma once

#include "ui.h"
#include "ui_text_box.h"
#include "ui_grid.h"
#include "ui_rectangle.h"
#include "ui_unstretched_text_box.h"

class ClockUI : public UI {
public:
    ClockUI(float aspect, int rounds);
    ~ClockUI();
private:
    std::string format_time(int seconds);
    void set_time(int seconds);

    UITextBox clock;
    UIGrid round_count_bg;
    UIUnstretchedTextBox ready;

    std::vector<UIRectangle*> round_counts;
    bool player_ready;
};