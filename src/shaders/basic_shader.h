#pragma once

#include "shader.h"

class BasicShader : public Shader {
public:
    virtual void init() override;
    virtual void pre_draw(SceneInfo &scene_info) override;
    virtual void post_draw() override;
    virtual void draw(Mesh *mesh, SceneInfo &scene_info,
                      glm::mat4 to_world) override;
};