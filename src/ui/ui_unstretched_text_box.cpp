#include <iostream>
#include "ui_unstretched_text_box.h"

// Some magic constants for properly spacing the text
#define X_SCALE_FACTOR 4.8f
#define Y_SCALE_FACTOR 4.8f
#define Y_OFFSET_FACTOR 10.f

UIUnstretchedTextBox::UIUnstretchedTextBox(float char_width, float char_height,
                       float start_x, float start_y,
                       float width, float height, float padding,
                       Alignment h_align, Alignment v_align,
                       Color text_color, Color bg_color,
                       float alpha) :
    UIContainer(start_x, start_y),
    char_width(char_width), char_height(char_height),
    real_char_width(char_width / X_SCALE_FACTOR), real_char_height(char_height / Y_SCALE_FACTOR),
    width(width), inner_width(width - 2 * padding),
    height(height), inner_height(height - 2 * padding),
    padding(padding),
    chars_per_line(static_cast<int>(inner_width / char_width)),
    max_lines(static_cast<int>(height / char_height)),
    h_align(h_align), v_align(v_align),
    text_color(text_color),
    bg(0, 0, width, height, bg_color, alpha) {

    attach(bg);
}

void UIUnstretchedTextBox::set_text(const std::string &text) {
    // Detach old text
    for (auto &node : texts)
        detach(*node);

    // Calculate the contents of new text nodes and push each line back
    unsigned long nchars = static_cast<int>(text.length()),
                  nlines = nchars / chars_per_line,
                  vspace = max_lines - nlines;

    float x_off = padding;

    // TODO(metakirby5) fix vertical alignment
    float y_off = -(inner_height / Y_OFFSET_FACTOR) + inner_height - calc_pad(v_align, char_height * vspace);

    std::string remaining = text;
    std::string line;
    for (unsigned long i = 0; i < max_lines; ++i) {
        if (remaining.empty()) break;

        std::tie(line, remaining) = break_before(remaining);
        texts.push_back(
                std::make_unique<UITextNode>(line, x_off + hpad(line), y_off,
                                             real_char_width, real_char_height,
                                             text_color));
        attach(*texts.back());
        y_off -= char_height;
    }
}

std::pair<std::string, std::string> UIUnstretchedTextBox::break_before(std::string text) {
    size_t cut_at = text.find_last_of(" ", static_cast<unsigned long>(chars_per_line));
    std::string line;

    if (cut_at != std::string::npos) {
        line = text.substr(0, cut_at);
        text = text.substr(cut_at);

        // Cut out the extra space at the next start of line
        size_t first_nonspace = text.find_first_not_of(" ");
        if (first_nonspace != std::string::npos)
            text = text.substr(first_nonspace);
    } else {
        line = text;
        text = "";
    }

    return std::make_pair(line, text);
}

float UIUnstretchedTextBox::calc_pad(Alignment align, float space) {
    switch (align) {
        case END:
            return space;
        case MIDDLE:
            return space / 2;
        case START:
        default:
            return 0.f;
    }
}

float UIUnstretchedTextBox::hpad(const std::string &line) {
    float space = inner_width - (line.length() * char_width);
    return calc_pad(h_align, space);
}
