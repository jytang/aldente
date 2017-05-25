#pragma once

#include "ui_element.h"

#include <glm/glm.hpp>
#include <GL/glew.h>

class UIImageNode : public UIElement {
public:
    UIImageNode(float start_x, float start_y,
                float width, float height,
                GLuint texture_id)
        : UIElement(start_x, start_y),
          width(width), height(height),
          texture_id(texture_id) {}

    void draw(Render2D &renderer_2d, float offset_x, float offset_y) override;
    void set_width(float width)   { this->width = width; }
    void set_height(float height) { this->height = height; }
    void set_image(GLuint image)  { this->texture_id = image; }
private:
    float width, height;
    GLuint texture_id;
};
