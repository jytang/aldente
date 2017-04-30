#include "raw_map.h"

namespace input {
bool operator<(const RawButton& l, const RawButton& r) {
    if (l.is_button != r.is_button)
        return l.is_button < r.is_button;
    return l.input < r.input;
}

bool operator<(const RawKeyboard& l, const RawKeyboard& r) {
    if (l.key != r.key)
        return l.key < r.key;
    return l.mods < r.mods;
    // Level is not used in lookup
}
}
