#include "ui.h"

#include "ui_grid.h"
#include "util/colors.h"

UI::UI() {
    root = UIContainer(0, 0);
}

void UI::draw() {
    root.draw(renderer_2d, 0, 0);
}

void UI::attach(UIElement &e) {
    root.attach(e);
}

void UI::detach(UIElement &e) {
    root.detach(e);
}

void UI::enable() {
    enabled = true;
    root.enable();
}

void UI::disable() {
    enabled = false;
    root.disable();
}
