#pragma once

#include "ui.h"
#include "ui_image_node.h"
#include "ui_rectangle.h"
#include "ui_unstretched_text_box.h"
#include "util/color.h"

#include <string>

class EffectsUI : public UI {
public:
    EffectsUI(float aspect);
private:
    void show_image(float seconds, std::string image_name);
    void show_rect(float seconds, Color c);
    void show_text(float seconds, std::string text);

    UIImageNode image;
    UIRectangle rect;
    UIUnstretchedTextBox text_box;
};