#pragma once

#include "material.h"
#include "skeleton.h"

class ModelFilter {
public:
    Skeleton skeleton;
    Material material;
    // Flags to indicate which material fields are being overwritten
    // There is certainly a better, more extensible way to do this
    bool override_diffuse = false;
    bool override_alpha = false;
    bool override_shadows = false;
};