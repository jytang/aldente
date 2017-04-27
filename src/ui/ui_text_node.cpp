#include "ui_text_node.h"

void UITextNode::draw(Render2D &renderer_2d,
                       float offset_x, float offset_y) {
    if (!enabled) return;
    renderer_2d.render_textP(text,
                             offset_x + start_x,
                             offset_y + start_y,
                             x_scale, y_scale,
                             color);
}

void UITextNode::set_text(std::string& t) {
    text = t;
}

