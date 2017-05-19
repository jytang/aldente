#include "ui_clock.h"

UIClock::UIClock(float start_x, float start_y,
                 float width, float height,
                 Color clock_color,
                 Color clock_bg_color)
    : UIContainer(start_x, start_y),
    width(width), height(height) {

    // Make clock bg a rectangle.
    clock_bg = UIRectangle(0, 0, width, height, clock_bg_color, 0.5f);
    attach(clock_bg);

    // Start clock at 00:00.
    clock_text = UITextNode(format_time(0), 0, 0, width / 10.f, height / 3.f, clock_color);
    attach(clock_text);
}

void UIClock::set_time(int seconds) {
    clock_text.set_text(format_time(seconds));
}

std::string UIClock::format_time(int seconds) {
    int hours = seconds / 3600;
    seconds %= 3600;
    int minutes = seconds / 60;
    seconds %= 60;

    char buf[128];

    if (hours == 0)
        sprintf(buf, "%02d:%02d", minutes, seconds);
    else
        sprintf(buf, "%02d:%02d:%02d", hours, minutes, seconds);
    return buf;
}