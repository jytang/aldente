#pragma once
#include "events/event.h"

namespace kuuhaku {
namespace events {

const int INPUT_ANALOG_LEVELS = 5;
struct InputData {
  int joystick;
  bool is_button;
  int which; // Button or axis number
  int level; // If button, zero is not pressed, nonzero is pressed.
             // Otherwise, is axis analog level.
};
// A joystick input.
class InputEvent : public Event<InputData> {};

}
}
