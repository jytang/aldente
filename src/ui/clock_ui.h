#pragma once

#include "ui_clock.h"
#include "ui.h"

class ClockUI : public UI {
public:
    ClockUI(float aspect);
private:
    UIClock clock;
};