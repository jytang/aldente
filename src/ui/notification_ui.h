#pragma once

#include <set>
#include <queue>
#include "ui.h"
#include "ui_unstretched_text_box.h"

class NotificationUI : public UI {
public:
    NotificationUI(float aspect, float width, float height);

private:
    float aspect_width, height;
    bool animating;

    std::shared_ptr<UIUnstretchedTextBox>  make_notification(std::string text);
    void show_next(); // Pop from queue and show next

    std::queue<std::string> queue;
    std::set<std::shared_ptr<UIUnstretchedTextBox>> notifications;

    // Hacky fix to prevent animation segfaults...
    std::set<std::shared_ptr<UIUnstretchedTextBox>> graveyard;
};
