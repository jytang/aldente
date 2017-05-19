#include "ui_text_box.h"

UITextBox::UITextBox(float start_x, float start_y,
                     float width, float height,
                     std::string text,
                     Color text_color, Color bg_color,
                     float alpha=1.f) :
    UIContainer(start_x, start_y),
    width(width), height(height) {

    bg = UIRectangle(0, 0, width, height, bg_color, alpha);
    attach(bg);

    // Set text node accordingly, fill bg as much as possible. (TODO: padding, centering params)
    text_node = UITextNode(text, 0, 0, width / 20.f, height / 20.f, text_color);
    attach(text_node);
}

void UITextBox::set_text(std::string &new_text) {
    text_node.set_text(new_text);
}