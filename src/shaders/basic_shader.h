#pragma once

#include "shader.h"

class BasicShader :
        public Shader {
public:
    BasicShader(GLuint shader_id);

    void set_material(Material *m);

    void draw(Geometry *g, glm::mat4 to_world);
};

