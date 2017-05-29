#include "dialog_ui.h"

DialogUI::DialogUI(float aspect, float width, float height, float padding)
    : bg(0, 0, aspect * 100.f, 100.f, Color::BLACK, 0.8f)
    , textbox(3.f, 4.f, aspect * (100.f - width) / 2, padding, aspect * width, height, padding,
              UIUnstretchedTextBox::START, UIUnstretchedTextBox::START, Color::WHITE, Color::BLACK, 0.5f){
    disable();
    attach(bg);
    attach(textbox);
    textbox.set_text("hello test this is a pretty long string i hope it will display correctly please help");
}
