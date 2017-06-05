#pragma once

#include "ui_container.h"
#include "ui_text_node.h"
#include "ui_rectangle.h"
#include "util/color.h"
#include <string>

// Uses this text box if you don't want the text to stretch to width.
// If text will overflow the box, it won't be displayed.
// Newlines are curently unsupported.
class UIUnstretchedTextBox : public UIContainer {
public:
    enum Alignment {
        START, MIDDLE, END,
    };
    UIUnstretchedTextBox(float char_width, float char_height,
               float start_x, float start_y,
               float width, float height,
               float padding,
               Alignment h_align, Alignment v_align,
               Color text_color, Color bg_color,
               float alpha, bool draw_bg = true);
    void set_text(const std::string &text);
    void set_color(Color color);
    unsigned long get_max_chars();
    void set_alpha(float alpha) override;

private:
    float char_width, char_height, line_height;
    float width, inner_width, height, inner_height, padding;
    unsigned long chars_per_line;
    int max_lines;
    Alignment h_align, v_align;
    Color text_color;
    UIRectangle bg;
    std::vector<std::unique_ptr<UITextNode>> texts;

    // Padding calculations
    float calc_pad(Alignment align, float space);
    float hpad(const std::string &line);
};