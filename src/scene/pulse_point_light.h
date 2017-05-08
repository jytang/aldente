#pragma once

class PulsePointLight : public PointLight {
private:
    float high_intensity, low_intensity, step;
public:
    PulsePointLight(glm::vec3 default_position,
                    Color color = Color::WHITE,
                    float low_intensity = 0.f,
                    float high_intensity = 0.5f,
                    float step = 0.1f) :
        PointLight(default_position, color, high_intensity),
        high_intensity(high_intensity),
        low_intensity(low_intensity),
        step(step) {}

    void update() {
        // Add intensity step every update loop.
        intensity -= step;
        // Reverse step direction (oscillate) upon hitting edges.
        if (intensity >= high_intensity || intensity <= low_intensity)
            step = -step;
    }
};