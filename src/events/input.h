#pragma once
#include <boost/signals2.hpp>

namespace kuuhaku {
namespace events {

using boost::signals2::signal;

const int INPUT_ANALOG_LEVELS = 5;
struct JoystickData {
  int id; // Which joystick
  bool is_button;
  int input; // Button or axis number
  int state; // If button, zero is not pressed, nonzero is pressed.
             // Otherwise, is axis analog level.
};
// A joystick input.
extern signal<void (JoystickData &)> joystick_event;

}
}
