#pragma once

#define GLFW_INCLUDE_GLEXT

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

#include "shader.h"

class ShaderManager {
private:
    static std::map<const char *, Shader *> shaders;
    static Shader *default_shader;
public:
    static void destroy();

    static void create_shader_program(const char *type);

    static Shader *get_shader_program(const char *type);

    static void set_default(const char *type);

    static Shader *get_default();
};

