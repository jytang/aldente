#pragma once
#include "boost/signals2.hpp"
#include "events.h"

namespace input {

// Modal indirection for input dispatching.
class ModalInput {
public:
    // The list of input modes.
    enum InputMode {
        NORMAL, DIALOG,
    };

    // A pair of signals for button and stick events.
    struct InputSignals {
        boost::signals2::signal<void(const events::ButtonData &)> buttons;
        boost::signals2::signal<void(const events::StickData &)> sticks;
    };

    ModalInput();

    // Singleton injection.
    static void provide(std::shared_ptr<ModalInput> p) { instance = p; }
    static std::shared_ptr<ModalInput> get() { return instance; }

    void set_mode(InputMode mode) { current_mode = mode; }

    // Used to subscribe to input signals.
    // Example:
    InputSignals &with_mode(InputMode mode) { return signals[mode]; };

private:
    static std::shared_ptr<ModalInput> instance;
    InputMode current_mode;
    std::map<InputMode, InputSignals> signals;
};

}
