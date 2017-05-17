#pragma once

#include "material.h"
#include "skeleton.h"

class ModelFilter {
public:
    Skeleton skeleton;
    Material material;
    bool material_filter = false;
};