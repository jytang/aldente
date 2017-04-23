#pragma once

#include "shader.h"
#include <vector>

class SkyboxShader :
        public Shader {
public:
    GLuint current_texture_id = 0;
    std::vector<GLuint> texture_ids;
    GLuint VAO, VBO;

    SkyboxShader(GLuint shader_id);

    void load_cubemap();

    void set_material(Material *m);

    void draw(Geometry *g, glm::mat4 to_world);
};

