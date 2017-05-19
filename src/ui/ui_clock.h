#pragma once

#include "ui_container.h"
#include "ui_text_node.h"
#include "ui_rectangle.h"
#include "util/color.h"
#include <string>

class UIClock : public UIContainer {
public:
    UIClock(float start_x, float start_y,
            float width, float height,
            Color clock_color,
            Color clock_bg_color);
    void set_time(int seconds);
private:
    float width, height;
    UIRectangle clock_bg;
    UITextNode clock_text;

    std::string format_time(int seconds);
};