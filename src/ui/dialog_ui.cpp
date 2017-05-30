#include <asset_loader.h>
#include "dialog_ui.h"

DialogUI::DialogUI(float aspect, float width, float height, float padding)
    : bg(0, 0, aspect * 100.f, 100.f, Color::BLACK, 0.8f)
    , textbox(3.f, 4.f,
              aspect * (100.f - width) / 2 + height, // Center, but make room for the portrait
              padding, aspect * width - height, height, padding,
              UIUnstretchedTextBox::START, UIUnstretchedTextBox::START, Color::WHITE, Color::BLACK, 0.5f)
    , portrait(aspect * (100.f - width) / 2, padding, height, height, AssetLoader::get_texture("dio.jpg")) {
    attach(bg);
    attach(portrait);
    attach(textbox);
    textbox.set_text("hellotestthisisaprettylongstringihope it will display correctly please help");
}
