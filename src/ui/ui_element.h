#pragma once

#include <vector>

#include "render2d.h"

class UIElement {
public:
    UIElement() {} // default ctor
    virtual ~UIElement() {}
    UIElement(float start_x, float start_y)
        : start_x(start_x), start_y(start_y) {}
    virtual void draw(Render2D &renderer_2d,
                      float offset_x, float offset_y) = 0;
    virtual void enable() { enabled = true; }
    virtual void disable() { enabled = false; }
    bool is_enabled() { return enabled; }
    virtual void attach(UIElement &child) {} // only containers implement this
    virtual void detach(UIElement &child) {} // only containers implement this
    virtual void do_selection() {} // "selectable" subclasses implement this
    virtual void do_action() {} // "actionable" subclasses implement this
    void set_start_x(float start_x) { this->start_x = start_x; }
    void set_start_y(float start_y) { this->start_y = start_y; }
protected:
    bool enabled = true;    // by default, UI elements are shown.
    float start_x, start_y; // starting coordinates (in UI units)
};
