#pragma once

#include "ui.h"
#include "ui_rectangle.h"

class MainMenuUI : public UI {
public:
    MainMenuUI(float aspect);
private:
    UIRectangle bg;
};