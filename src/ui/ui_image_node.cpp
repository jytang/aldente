#include "ui_image_node.h"

#include "util/color.h"

void UIImageNode::draw(Render2D &renderer_2d,
                       float offset_x, float offset_y) {
    if (!enabled) return;
    renderer_2d.render_rectP(offset_x + start_x,
                             offset_y + start_y,
                             width, height,
                             Color::WHITE, // color goes unused in shader
                             texture_id,
                             alpha); // image nodes no transparency
}

