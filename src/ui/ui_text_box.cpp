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
    float text_width = width / text.length() / X_SCALE_FACTOR;
    float text_height = height / Y_SCALE_FACTOR;
    float y_offset = height / Y_OFFSET_FACTOR;
    text_node = UITextNode(text, 0, y_offset, text_width, text_height, text_color);
    attach(text_node);

    // Setup debug listener
    events::debug::toggle_ui_text_box_background_event.connect([&]{
        this->alpha = this->alpha == 0.99f ? initial_alpha : 0.99f;
        bg.set_alpha(this->alpha);
    });
}

void UITextBox::set_text(std::string new_text) {
    text_node.set_text(new_text);

    // Adjust text_width accordingly.
    text_node.set_x_scale(width / new_text.length() / X_SCALE_FACTOR);
}