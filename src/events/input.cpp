#include "events/input.h"
#include <boost/signals2.hpp>

namespace kuuhaku {
namespace events {

using boost::signals2::signal;
signal<void (JoystickData &)> joystick_event;

}
}
