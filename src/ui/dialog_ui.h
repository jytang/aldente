#pragma once

#include "ui.h"
#include "ui_unstretched_text_box.h"
#include "ui_image_node.h"
#include <boost/signals2.hpp>

// Fully modal, so covers the screen. Should have highest z-index, except for like a pausse menu.
// Show a sequence of dialog with events::ui::show_dialog({{portrait_str, text}, ...}).
class DialogUI : public UI {
public:
    typedef std::pair<std::string, std::string> str_pair;

    DialogUI(float aspect, float width, float height, float padding);

private:
    UIRectangle bg;
    UIUnstretchedTextBox textbox;
    UIImageNode portrait;

    // Variables related to current dialog
    boost::signals2::connection button_conn;
    std::vector<str_pair> current_dialog;
    std::string display, current, remaining;
    bool animating;
    std::function<void()> cancel_text_animation;
    std::function<void()> do_after;

    // Helper functions
    void display_next();
    void skip_animation();
    void stop_display();
};
