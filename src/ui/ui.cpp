#include "ui.h"

UI::UI(float start_x, float start_y)  {
    root = UIContainer(start_x, start_y);
}

void UI::draw(Render2D &renderer_2d) {
    if (enabled) root.draw(renderer_2d, 0, 0);
}

void UI::attach(UIElement &e) {
    root.attach(e);
}

void UI::detach(UIElement &e) {
    root.detach(e);
}

void UI::enable() {
    enabled = true;
}

void UI::enable_all() {
    enabled = true;
    root.enable();
}

void UI::disable() {
    enabled = false;
}

void UI::disable_all() {
    enabled = false;
    root.disable();
}

void UI::toggle() {
    enabled = !enabled;
}

void UI::enable_animated() {
    enable();
    root.animate_alpha(1.f, 1.f);
}

void UI::disable_animated() {
    root.animate_alpha(0.f, 1.f, [&](){
        disable();
    });
}