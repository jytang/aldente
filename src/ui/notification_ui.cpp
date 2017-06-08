#include <events.h>
#include <timer.h>
#include <iostream>
#include "notification_ui.h"

NotificationUI::NotificationUI(float aspect, float width, float height)
    : aspect_width(aspect * width), height(height), animating(false) {

    // Show notification when requested
    events::ui::show_notification.connect([&](const std::string &message, float duration) {
        que.emplace(message, duration);
        show_next();
    });
}

std::shared_ptr<UIUnstretchedTextBox> NotificationUI::make_notification(const std::pair<std::string, float> &spec) {
    // Create a notification off-screen
    std::shared_ptr<UIUnstretchedTextBox> notif = std::make_shared<UIUnstretchedTextBox>(
            3.f, 4.f, 0, -height, aspect_width, height,
            2.f, UIUnstretchedTextBox::START, UIUnstretchedTextBox::START,
            Color::WHITE, Color::BLACK, .5f, true, false
    );
    notif->set_text(spec.first);
    attach(*notif);
    notifications.insert(notif);

    // Fade it out and remove it after 1 second
    Timer::get()->do_after(std::chrono::milliseconds(static_cast<int>(1000 * spec.second)), [&, notif]() {
        notif->animate_alpha(0, .5f, [&, notif]() {
            detach(*notif);
            notifications.erase(notif);
            graveyard.insert(notif);
        });
    });

    return notif;
}

void NotificationUI::show_next() {
    if (animating || que.empty()) return;

    auto notif = make_notification(que.front());
    que.pop();

    // Shift all notifications up by height, then bring in the next notification
    // I sure am glad we're single-threaded
    animating = true;
    for (auto cur : notifications) {
        // Once the most recent notification has been shown, bring in the next notification
        std::function<void()> callback = [](){};
        if (cur == notif)
            callback = [&]() {
                animating = false;
                show_next();
            };

        cur->animate_relative(0, height, .2f, callback);
    }
}
