#pragma once

#include "conceptual_translator.h"
#include "events.h"

namespace input {

typedef std::map<events::Stick, std::pair<events::ConceptualButton, events::ConceptualButton>> StickMap;

const StickMap STICKS_DEFAULT = {
        {events::STICK_LEFT, {events::AX_LH, events::AX_LV}},
        {events::STICK_RIGHT, {events::AX_RH, events::AX_RV}},
};

class AxisCombiner {
public:
    AxisCombiner(const ConceptualTranslator &translator, const StickMap &sticks);

private:
    const ConceptualTranslator &translator;
    const StickMap &sticks;

    std::map<events::ConceptualButton, events::Stick> axis_to_stick;
};

}
