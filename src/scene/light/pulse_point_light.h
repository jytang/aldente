#pragma once

#include "timer.h"

class PulsePointLight : public PointLight {
private:
    float high_intensity, low_intensity, step;
    std::function<void()> cancel_pulse;
    bool enabled = false;
public:
    PulsePointLight(glm::vec3 default_position,
                    Color color = Color::WHITE,
                    float low_intensity = 0.f,
                    float high_intensity = 0.5f,
                    float step = 0.1f) :
        PointLight(default_position, color, high_intensity),
        high_intensity(high_intensity),
        low_intensity(low_intensity),
        step(step) {

        enable(); // enable pulsing by default
    }

    void enable() {
        if (enabled) return; // already enabled
        enabled = true;

        // Setup timed pulse, at 30 ms steps
        cancel_pulse = Timer::get()->do_every(std::chrono::milliseconds(30), [&]() {
            // Add intensity step every update loop.
            intensity -= step;
            // Reverse step direction (oscillate) upon hitting edges.
            if (intensity >= high_intensity || intensity <= low_intensity)
                step = -step;
        });
    }

    void disable() {
        if (!enabled) return; // already disabled
        enabled = false;
        cancel_pulse();
    }
};