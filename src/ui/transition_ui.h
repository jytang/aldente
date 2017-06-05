#pragma once

#include "ui.h"
#include "ui_rectangle.h"
#include "ui_image_node.h"
#include "game/direction.h"
#include "util/color.h"

#include <vector>
#include "ui_rectangle.h"

class TransitionUI : public UI {
public:
    TransitionUI(float aspect);
private:
    void fade(float seconds, Color c = Color::BLACK,
              std::function<void()> do_apex=[](){});
    void wipe(float seconds, Color c = Color::BLACK, Direction dir = Direction::LEFT,
              std::function<void()> do_apex=[](){});

    void wipe_reset();

    UIRectangle bg2;
    UIImageNode bg;
    float aspect;
    float initial_start_x, initial_start_y;
    static std::vector<Color> transition_colors;
};