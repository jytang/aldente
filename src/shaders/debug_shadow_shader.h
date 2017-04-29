#pragma once

#include "shader.h"

class DebugShadowShader : public Shader {
private:
    GLuint VAO, VBO;
    static SceneInfo temp = { NULL, glm::vec3(1.f) }; // hacky, but works.
public:
    virtual void init() override;
    virtual void pre_draw(SceneInfo &scene_info) override;
    virtual void post_draw() override;
    virtual void draw(Mesh *mesh = NULL, SceneInfo &scene_info = temp,
                      glm::mat4 to_world = glm::mat4(1.f)) override;
};