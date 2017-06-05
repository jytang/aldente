#include <events.h>
#include "main_menu_ui.h"

#include "timer.h"
#include "util/color.h"

MainMenuUI::MainMenuUI(float aspect)
        : UI(0.f * aspect, 0.f),
          bg(0, 0,
             100.f * aspect, 100.f,
             Color::WHITE, 1.f) {
    disable();
    attach(bg);
}