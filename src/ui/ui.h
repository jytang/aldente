#pragma once

#include "render2d.h"
#include "ui_container.h"

class UI {
public:
    UI(float start_x = 0.f, float start_y = 0.f);
    void draw(Render2D &renderer_2d);
    void attach(UIElement &e);
    void detach(UIElement &e);
    void enable();  // Enables visibility of top level UI.
    void disable(); // Disables visibliity
    void enable_all();  // Enables visibility of all contained UI elts.
    void disable_all(); // Disables visibliity.
    void toggle();
    virtual void enable_animated();
    virtual void disable_animated();
protected:
    bool enabled = true;    // by default, UI elements are shown.
    UIContainer root;
};
