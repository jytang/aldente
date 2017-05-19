#pragma once

#include "game_object.h"
#include "game/construct_types.h"
#include "construct.h"

#include <map>

class ConstructPreview : public GameObject {
public:
    void set_construct_type(ConstructType type);
    Construct *curr_preview;
private:
    template <typename T>
    T* make_preview();

    // make the previews lazily and fu<k;ng cache them
    std::map<ConstructType, Construct *> cached_previews;
};
