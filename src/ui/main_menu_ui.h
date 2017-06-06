#pragma once

#include "ui.h"
#include "ui_rectangle.h"
#include "ui_text_box.h"
#include "ui_image_node.h"

class MainMenuUI : public UI {
public:
    MainMenuUI(float aspect);
    void enable_animated() override;
    void disable_animated() override;
private:
    UIRectangle bg;
    UIImageNode logo;
};