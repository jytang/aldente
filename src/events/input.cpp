#include "events/input.h"

namespace kuuhaku {
    namespace events {

        using boost::signals2::signal;
        signal<void(JoystickData &)> joystick_event;

    }
}
