#include <events.h>
#include "main_menu_ui.h"

#include "timer.h"
#include "util/color.h"

MainMenuUI::MainMenuUI(float aspect)
        : UI(20.f * aspect, 30.f),
          bg(0, 0,
             30.f * aspect, 40.f,
             Color::BLACK, 0.5f) {
    disable();
    attach(bg);
}