#pragma once

#include "ui_container.h"
#include "ui_text_node.h"
#include "ui_rectangle.h"
#include "util/color.h"
#include <string>

class UITextBox : public UIContainer {
public:
    UITextBox(std::string text,
              float start_x, float start_y,
              float width, float height,
              Color text_color, Color bg_color = Color::WHITE,
              float alpha = 0.f);
    void set_text(std::string new_text);
    void set_width(float width);
    void set_height(float height);
    void set_text_color(Color text_color);
    void set_bg_color(Color bg_color);
    void set_alpha(float alpha) override;

private:
    float calc_text_width(std::string s);

    float width, height;
    float initial_alpha;
    UIRectangle bg;
    UITextNode text_node;
};