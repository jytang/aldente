#pragma once

#include "ui.h"
#include "ui_text_box.h"

class ClockUI : public UI {
public:
    ClockUI(float aspect);
private:
    std::string format_time(int seconds);
    void set_time(int seconds);

    UITextBox *clock;
};