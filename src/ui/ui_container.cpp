#include "ui_container.h"

#include <algorithm>
#include "timer.h"

void UIContainer::attach(UIElement &child) {
    children.push_back(&child);
}

void UIContainer::detach(UIElement &child) {
    auto it = std::find(children.begin(), children.end(), &child);
    if (it != children.end()) children.erase(it);
}

void UIContainer::enable() {
    enabled = true;
    for (auto it = children.begin(); it != children.end(); ++it) {
        (*it)->enable();
    }
}

void UIContainer::disable() {
    enabled = false;
    for (auto it = children.begin(); it != children.end(); ++it) {
        (*it)->disable();
    }
}

// Draw container's children by adjusting their offset by
// the container coordinates.
void UIContainer::draw(Render2D &renderer_2d,
              float offset_x, float offset_y) {
    if (!enabled) return; // don't draw anything if the container is disabled
    for (auto it = children.begin(); it != children.end(); ++it) {
        (*it)->draw(renderer_2d, start_x + offset_x, start_y + offset_y);
    }
}

void UIContainer::set_alpha(float alpha) {
    UIElement::set_alpha(alpha);
    for (auto it = children.begin(); it != children.end(); ++it) {
        (*it)->set_alpha(alpha);
    }
}