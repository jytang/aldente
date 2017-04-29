#pragma once

#include "shader.h"

class DebugShadowShader : public Shader {
private:
    GLuint VAO, VBO;
public:
    virtual void init() override;
    virtual void pre_draw(SceneInfo &scene_info) override;
    virtual void post_draw() override;
    virtual void draw(Mesh *mesh, SceneInfo &scene_info,
                      glm::mat4 to_world = glm::mat4(1.f)) override;
};
