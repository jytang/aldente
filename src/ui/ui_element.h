#pragma once

#include <vector>

#include "render2d.h"

class UIElement {
public:
    UIElement() {} // default ctor
    virtual ~UIElement() {}
    UIElement(float start_x, float start_y, float alpha=1.f)
        : start_x(start_x), start_y(start_y), alpha(alpha),
          cancel_animation([](){}),
          cancel_alpha_animation([](){}) {}
    virtual void draw(Render2D &renderer_2d,
                      float offset_x, float offset_y) = 0;
    virtual void enable() { enabled = true; }
    virtual void disable() { enabled = false; }
    bool is_enabled() { return enabled; }
    virtual void attach(UIElement &child) {} // only containers implement this
    virtual void detach(UIElement &child) {} // only containers implement this
    virtual void do_selection() {} // "selectable" subclasses implement this
    virtual void do_action() {} // "actionable" subclasses implement this
    void set_start_x(float start_x)     { this->start_x = start_x; }
    void set_start_y(float start_y)     { this->start_y = start_y; }
    virtual void set_alpha(float alpha) { this->alpha = alpha; }

    // Animate with an absolute translation, with an optional callback called afterwards.
    void animate_to(float target_x, float target_y,
                    float seconds,
                    std::function<void()> do_after=[](){});
    // Animate with a relative translation, mostly a convenience function as it calls the above.
    void animate_relative(float delta_x, float delta_y,
                 float seconds,
                 std::function<void()> do_after=[](){});

    void animate_alpha(float target_alpha, float seconds,
                       std::function<void()> do_after=[](){});

protected:
    bool enabled = true;    // by default, UI elements are shown.
    float start_x, start_y; // starting coordinates (in UI units)
    float alpha;            // alpha channel of ui element.
private:
    bool moving_away(float current, float delta, float target); // helper for animation
    std::function<void()> cancel_alpha_animation; // timer cancellation function
    std::function<void()> cancel_animation; // timer cancellation function
};
