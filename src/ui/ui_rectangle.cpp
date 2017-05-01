#include "ui_rectangle.h"

void UIRectangle::draw(Render2D &renderer_2d,
                       float offset_x, float offset_y) {
    if (!enabled) return;
    renderer_2d.render_rectP(offset_x + start_x,
                             offset_y + start_y,
                             width, height,
                             color,
                             0, // 0 is passed to indicate that no texture is bound
                             alpha);
}

