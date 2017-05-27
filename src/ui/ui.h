#pragma once

#include "render2d.h"
#include "ui_container.h"

class UI {
public:
    UI(float start_x = 0.f, float start_y = 0.f);
    void draw();
    void attach(UIElement &e);
    void detach(UIElement &e);
    void enable();  // Enables visibility of top level UI.
    void disable(); // Disables visibliity
    void enable_all();  // Enables visibility of all contained UI elts.
    void disable_all(); // Disables visibliity.
    void toggle();
protected:
    bool enabled = true;    // by default, UI elements are shown.
private:
    Render2D renderer_2d;
    UIContainer root;
};
