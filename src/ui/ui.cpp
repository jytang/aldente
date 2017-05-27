#include "ui.h"

#include "ui_grid.h"

UI::UI(float start_x, float start_y)  {
    root = UIContainer(start_x, start_y);
}

void UI::draw() {
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