#pragma once

namespace input {
// Comparators are for std::map usage.

struct RawButton {
    bool is_button;
    int input; // Button or axis number
};
bool operator<(const RawButton& l, const RawButton& r);

struct RawKeyboard {
    int key;
    int mods;
    int level; // Defines the analog level for the keypress
};
bool operator<(const RawKeyboard& l, const RawKeyboard& r);
}
