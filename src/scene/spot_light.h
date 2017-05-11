#pragma once

#include "light.h"

class SpotLight : public Light {
private:
    glm::vec3 default_position;
    glm::vec3 default_direction;
public:
    SpotLight(glm::vec3 default_position,
              glm::vec3 default_direction,
              Color color = Color::WHITE,
              float angle = glm::radians(20.0f), // half angle of beam opening
              float taper = 20.f, // exponential degree to which light tapers at the edges
              float intensity = 1.f,
              float quadratic = 0.1f, // 10% inverse square falloff
              float ambient_coeff = 0.07f) // standard 7% ambience
            : Light(intensity, ambient_coeff, color),
              default_position(default_position),
              default_direction(default_direction),
              angle(angle), taper(taper), quadratic(quadratic) {}

    glm::vec3 get_position() {
        return glm::vec3(transform.get_world_mat() * glm::vec4(default_position, 0.f));
    }

    glm::vec3 get_direction() {
        return glm::vec3(transform.get_world_mat() * glm::vec4(default_direction, 0.f));
    }

    float quadratic;
    float angle;
    float taper;
};
