#pragma once

#include "ui_element.h"
#include "util/color.h"

#include <glm/glm.hpp>

class UIRectangle : public UIElement {
public:
    UIRectangle() {} // default ctor
    UIRectangle(float start_x, float start_y,
                float width, float height,
                Color color, float alpha=1.f)
        : UIElement(start_x, start_y),
          width(width), height(height),
          color(color), alpha(alpha) {}

    void draw(Render2D &renderer_2d, float offset_x, float offset_y) override;
    void set_color(Color color)   { this->color = color; }
    void set_alpha(float alpha)   { this->alpha = alpha; }
    void set_width(float width)   { this->width = width; }
    void set_height(float height) { this->height = height; }
private:
    float width, height;
    float alpha;
    Color color;
};
