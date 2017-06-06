#pragma once

#include "ui.h"
#include "ui_unstretched_text_box.h"

class CountdownUI : public UI {
public:
    CountdownUI(float aspect);

private:
    UIUnstretchedTextBox count;
    std::function<void()> cur_timer;

    // Creater of lambdas. Fear me.
    std::function<void()> make_counter(const std::string &text, const std::function<void()> &do_after);
};
