#pragma once

#include <functional>
#include "ui_element.h"

class UIContainer : public UIElement {
public:
    UIContainer() {} // default ctor
    UIContainer(float start_x, float start_y, float alpha=1.f)
        : UIElement(start_x, start_y, alpha) {}

    virtual void attach(UIElement &child) override;
    virtual void detach(UIElement &child) override;
    virtual void enable() override;
    virtual void disable() override;
    virtual void set_alpha(float alpha) override;

    virtual void draw(Render2D &renderer_2d,
        float offset_x, float offset_y) override;
protected:
    std::vector<UIElement *> children;
};
