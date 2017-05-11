#pragma once

#include "light.h"

class DirectionalLight : public Light {
private:
    glm::vec3 default_direction;
public:
    DirectionalLight(glm::vec3 default_direction,
                     Color color = Color::WHITE,
                     float intensity = 1.f,
                     float ambient_coeff = 0.07f) // standard 7% ambience
            : Light(intensity, ambient_coeff, color),
              default_direction(default_direction) {}

    glm::vec3 get_direction() {
        return glm::vec3(transform.get_world_mat() * glm::vec4(default_direction, 0.f));
    }
};
