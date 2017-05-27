#pragma once

#include <asset_loader.h>
#include "ui_container.h"
#include "ui_text_box.h"
#include "ui_image_node.h"

class UIControllerLegend : public UIContainer {
public:
    UIControllerLegend(std::string texture, std::string text,
                       float start_x, float start_y,
                       float width, float height,
                       Color text_color, Color bg_color,
                       float alpha);
private:
    UIImageNode button_img;
    UITextBox text_box;
};
