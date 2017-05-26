#pragma once

#include "ui.h"
#include "ui_text_box.h"

class FPSUI : public UI {
public:
    FPSUI(float aspect);
private:
    UITextBox fps_display;
};