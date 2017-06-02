#include "ui_halo_container.h"

#include "util/color.h"

UIHaloContainer::UIHaloContainer(float start_x, float start_y,
        float element_width, float element_height,
        float selection_halo_padding)
    : UIContainer(start_x, start_y) {

    halo = UIRectangle(-selection_halo_padding, -selection_halo_padding,
                        element_width + selection_halo_padding * 2,
                        element_height + selection_halo_padding * 2,
                        Color::WINDWAKER_SAND);
    halo.disable(); // disabled by default
    if (selection_halo_padding != 0)
        children.push_back(&halo);
}

void UIHaloContainer::do_selection() {
    if (halo.is_enabled())
        halo.disable();
    else
        halo.enable();
}
