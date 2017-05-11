#pragma once

#include "light.h"

class PointLight : public Light {
private:
    glm::vec3 default_position;
public:
    PointLight(glm::vec3 default_position,
               Color color = Color::WHITE,
               float intensity = 0.5f,
               float quadratic = 0.07f, // 7% inverse square falloff
               float ambient_coeff = 0.07f) // standard 7% ambience
            : Light(intensity, ambient_coeff, color),
              default_position(default_position),
              quadratic(quadratic) {}

    glm::vec3 get_position() {
        return glm::vec3(transform.get_world_mat() * glm::vec4(default_position, 1.f));
    }

    float quadratic; // quadratic attenuation factor for inverse square falloff
    // read more here: https://developer.valvesoftware.com/wiki/Constant-Linear-Quadratic_Falloff
};