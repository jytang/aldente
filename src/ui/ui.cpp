#include "ui.h"

#include "ui_grid.h"
#include "util/colors.h"

UI::UI() {
    root = UIContainer(0, 0);
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
