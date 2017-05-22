#include "ui_text_box.h"

#include "events.h"

#define X_SCALE_FACTOR 4.8f
#define Y_SCALE_FACTOR 4.8f
#define Y_OFFSET_FACTOR 10.f

// TODO: support custom padding
UITextBox::UITextBox(std::string text,
                     float start_x, float start_y,
                     float width, float height,
                     Color text_color, Color bg_color,
                     float alpha) :
    UIContainer(start_x, start_y),
    width(width), height(height),
    alpha(alpha), initial_alpha(alpha) {

    bg = UIRectangle(0, 0, width, height, bg_color, alpha);
    attach(bg);

    // Set text node accordingly, fill bg as much as possible.
    float text_width = calc_text_width(text);
    float text_height = height / Y_SCALE_FACTOR;
    float y_offset = height / Y_OFFSET_FACTOR;
    text_node = UITextNode(text, 0, y_offset, text_width, text_height, text_color);
    attach(text_node);

    // Setup debug listener
    events::debug::toggle_ui_text_box_background_event.connect([&]{
        this->alpha = this->alpha == 1 ? initial_alpha : 1;
        bg.set_alpha(this->alpha);
    });
}

void UITextBox::set_text(std::string new_text) {
    text_node.set_text(new_text);

    // Adjust text_width accordingly.
    text_node.set_x_scale(calc_text_width(new_text));
}

void UITextBox::set_width(float width) {
    this->width = width;
    bg.set_width(width);
    text_node.set_x_scale(calc_text_width(text_node.get_text()));
}

void UITextBox::set_height(float height) {
    this->height = height;
    bg.set_height(height);
    text_node.set_y_scale(height / Y_SCALE_FACTOR);
}

void UITextBox::set_text_color(Color text_color) {
    text_node.set_color(text_color);
}

void UITextBox::set_bg_color(Color bg_color) {
    bg.set_color(bg_color);
}

void UITextBox::set_alpha(float alpha) {
    bg.set_alpha(alpha);
}

float UITextBox::calc_text_width(std::string s) {
    return (width / s.length()) / X_SCALE_FACTOR;
}