#pragma once

#include "ui_element.h"

#include <glm/glm.hpp>

class UIRectangle : public UIElement {
public:
    UIRectangle() {} // default ctor
    UIRectangle(float start_x, float start_y,
                float width, float height,
                glm::vec3 color, float alpha=1.f)
        : UIElement(start_x, start_y),
          width(width), height(height),
          color(color), alpha(alpha) {}

    void draw(Render2D &renderer_2d, float offset_x, float offset_y) override;
    void set_color(glm::vec3 color) { this->color = color; }
    void set_alpha(float alpha) { this->alpha = alpha; }
private:
    float width, height;
    float alpha;
    glm::vec3 color;
};
