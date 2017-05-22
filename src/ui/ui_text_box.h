#pragma once

#include "ui_container.h"
#include "ui_text_node.h"
#include "ui_rectangle.h"
#include "util/color.h"
#include <string>

class UITextBox : public UIContainer {
public:
    UITextBox() {} // default ctor
    UITextBox(float start_x, float start_y,
              float width, float height,
              std::string text,
              Color text_color,
              Color bg_color,
              float alpha);
    void set_text(std::string new_text);
private:
    float width, height;
    UIRectangle bg;
    UITextNode text_node;
};