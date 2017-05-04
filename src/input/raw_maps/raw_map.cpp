#include "raw_map.h"
#include <tuple>

namespace input {
bool operator<(const RawButton& l, const RawButton& r) {
    return std::tie(l.is_button, l.input) < std::tie(r.is_button, r.input);
}

bool operator<(const RawKeyboard& l, const RawKeyboard& r) {
    return std::tie(l.mods, l.key) < std::tie(r.mods, r.key);
    // Level is not used in lookup
}
}
