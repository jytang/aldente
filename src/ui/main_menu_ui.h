#pragma once

#include "ui.h"
#include "ui_rectangle.h"
#include "ui_text_box.h"

class MainMenuUI : public UI {
public:
    MainMenuUI(float aspect);
private:
    UIRectangle bg;
};