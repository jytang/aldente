#pragma once

#include "ui_element.h"
#include "util/color.h"

#include <string>

class UITextNode : public UIElement {
public:
    UITextNode() {} // default ctor
    UITextNode(std::string text,
               float start_x, float start_y,
               float x_scale, float y_scale,
               Color color)
        : text(text), UIElement(start_x, start_y),
          x_scale(x_scale), y_scale(y_scale),
          color(color) {}
    void draw(Render2D &renderer_2d, float offset_x, float offset_y) override;
    void set_text(std::string text) { this->text = text; }
    void set_x_scale(float x_scale) { this->x_scale = x_scale; }
    void set_y_scale(float y_scale) { this->y_scale = y_scale; }
    void set_color(Color color) { this->color = color; }
    std::string get_text() { return text; }
private:
    std::string text;
    float x_scale, y_scale;
    Color color;
};
