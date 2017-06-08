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
    void show_text(float seconds, std::string text);
    void show_rect(float seconds, Color c);
    void hide_image(float seconds);
    void hide_text(float seconds);
    void hide_rect(float seconds);

    void image_sequence(float seconds, std::string image_name);
    void text_sequence(float seconds, std::string text);
    void rect_sequence(float seconds, Color c);

    UIImageNode image;
    UIRectangle rect;
    UIUnstretchedTextBox text_box;
};