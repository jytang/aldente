#pragma once

#include "ui_container.h"
#include "ui_text_node.h"
#include "ui_rectangle.h"
#include "util/color.h"
#include <string>

class UITextBox : public UIContainer {
public:
    UITextBox() {} // default ctor
    UITextBox(std::string text,
              float start_x, float start_y,
              float width, float height,
              Color text_color, Color bg_color = Color::WHITE,
              float alpha = 0.f);
    void set_text(std::string new_text);
private:
    float width, height;
    float alpha, initial_alpha;
    UIRectangle bg;
    UITextNode text_node;
};