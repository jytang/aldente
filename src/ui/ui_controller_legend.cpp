#include "ui_controller_legend.h"
#include "events.h"

UIControllerLegend::UIControllerLegend(std::string texture, std::string text,
                                       float start_x, float start_y,
                                       float width, float height,
                                       Color text_color, Color bg_color,
                                       float alpha) :
        UIContainer(start_x, start_y),
        // Icon shall be a square at the left.
        button_img(start_x, start_y,
                   height, height, // Square
                   AssetLoader::get_texture(texture)),
        text_box(text,
                 start_x + height, // Start past the icon
                 start_y,
                 width - height,   // Width minus width of icon
                 height, text_color, bg_color, alpha) {
    attach(button_img);
    attach(text_box);
}
