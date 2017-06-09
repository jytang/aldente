#pragma once

#include "game_object.h"
#include "game/construct_types.h"
#include "construct.h"

#include <map>

class ConstructPreview : public GameObject {
public:
    ConstructPreview();
    void set_construct_type(ConstructType type, bool valid);
    void set_valid(bool valid);
    Construct *curr_preview;
private:
    template <typename T>
    T* make_preview();

    // make the previews lazily and fu<k;ng cache them
    std::map<ConstructType, Construct *> cached_previews;
};
