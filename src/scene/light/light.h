#pragma once

#include <glm/glm.hpp>

#include "util/color.h"
#include "transform.h"

class Light {
public:
    Light(float intensity, float ambient_coeff, Color color)
            : intensity(intensity),
              ambient_coeff(ambient_coeff),
              color(color) {}

    virtual void update() {}

    Transform transform;
    float intensity;
    float ambient_coeff;
    Color color;
};