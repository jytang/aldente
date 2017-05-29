#pragma once

#include "scene/scene.h"
#include "window.h"

/*
    Handles low level calls needed for running shadows
*/
class Shadows {
private:
    int screen_width, screen_height;

    bool debug_shadows_on;

public:
    Shadows();

    void shadow_pass(Scene *scene);

    void debug_shadows(Scene *scene);
};
