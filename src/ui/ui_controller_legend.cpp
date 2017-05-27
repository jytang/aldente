#include "ui_controller_legend.h"
#include "events.h"

static const float IMAGE_PADDING = 0.5f;

UIControllerLegend::UIControllerLegend(std::string texture, std::string text,
                                       float start_x, float start_y,
                                       float width, float height,
                                       float padding,
                                       Color text_color, Color bg_color,
                                       float alpha) :
        UIContainer(start_x, start_y),
        // Icon shall be a square at the left.
        button_bg(0, 0,
                  height + padding, height, // Square + padding
                  bg_color, alpha),
        button_img(IMAGE_PADDING, IMAGE_PADDING,
                   height - IMAGE_PADDING * 2, height - IMAGE_PADDING * 2, // Square with image padding
                   AssetLoader::get_texture(texture)),
        text_box(text,
                 padding + height, // Start past the icon
                 0,
                 width - height,   // Width minus width of icon
                 height, text_color, bg_color, alpha) {
    attach(button_bg);
    attach(button_img);
    attach(text_box);
}
