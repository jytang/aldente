#pragma once

#include "ui.h"
#include "ui_rectangle.h"
#include "ui_image_node.h"
#include "game/direction.h"
#include "util/color.h"

#include <vector>

class TransitionUI : public UI {
public:
    TransitionUI(float aspect);
private:
    void fade(float seconds, Color c = Color::BLACK,
              std::function<void()> do_apex=[](){});
    void wipe(float seconds, Color c = Color::BLACK, Direction dir = Direction::LEFT,
              std::function<void()> do_apex=[](){});

    void wipe_reset();

    UIImageNode bg;
    float aspect;
    static std::vector<Color> transition_colors;
};