#pragma once

#include "aldente.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

class AldenteServer : public Aldente {
public:
	~AldenteServer();
    void start();
};
