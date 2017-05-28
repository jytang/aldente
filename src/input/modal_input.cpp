#include "modal_input.h"

namespace input {

std::shared_ptr<ModalInput> ModalInput::instance;

ModalInput::ModalInput() : current_mode(NORMAL) {
    events::button_event.connect([&](const events::ButtonData &d) {
        with_mode(current_mode).buttons(d);
    });
    events::stick_event.connect([&](const events::StickData &d) {
        with_mode(current_mode).sticks(d);
    });
}

}
