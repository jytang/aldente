#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdlib.h>
#include <iostream>

#include "aldente.h"

/*
    Main Game Function.
    Game loop and scene setup happens here
*/
class AldenteClient : public Aldente {
public:
    ~AldenteClient();

    void start(bool);    // Main Game Loop
};
