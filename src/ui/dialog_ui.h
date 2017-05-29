#pragma once

#include "ui.h"
#include "ui_unstretched_text_box.h"

// Fully modal, so covers the screen. Should have highest z-index, except for like a pausse menu.
class DialogUI : public UI {
public:
    // TODO(metakirby5) use this to set portrait and text
    typedef std::vector<std::pair<std::string, std::string>> str_pair;

    DialogUI(float aspect, float width, float height, float padding);

private:
    UIRectangle bg;
    UIUnstretchedTextBox textbox;
};
