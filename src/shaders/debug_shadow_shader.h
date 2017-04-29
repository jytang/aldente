#pragma once

#include "shader.h"

class DebugShadowShader : public Shader {
public:
    virtual void draw(Mesh &mesh, SceneCamera &camera,
                      glm::mat4 model_to_world) override;
};