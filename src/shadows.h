#pragma once

#include "scene/scene.h"

/*
	Handles low level calls needed for running shadows
*/
class Shadows {
private:
    Shadows();

public:
    static Shadows *shadows;

    ~Shadows();

    void shadow_pass(Scene *scene);

    void debug_shadows();
};

