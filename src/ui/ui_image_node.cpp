#include "ui_image_node.h"

void UIImageNode::draw(Render2D &renderer_2d,
                       float offset_x, float offset_y) {
    if (!enabled) return;
    renderer_2d.render_rectP(offset_x + start_x,
                             offset_y + start_y,
                             width, height,
                             glm::vec3(1.f), texture_id);
}

