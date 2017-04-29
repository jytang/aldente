#pragma once

#include "shader.h"

class ShadowShader : public Shader {
public:
    virtual void init() override;
    virtual void pre_draw(SceneInfo &scene_info) override;
    virtual void post_draw() override;
    virtual void draw(Mesh *mesh, SceneInfo &scene_info,
                      glm::mat4 to_world) override;

    GLuint FBO, shadow_map_tex;
    unsigned int shadow_map_size;
    glm::mat4 light_matrix;
};

