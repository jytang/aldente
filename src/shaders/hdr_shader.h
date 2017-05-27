#pragma once

#include "shader.h"

class HDRShader : public Shader {
public:
    virtual void init() override;
    virtual void pre_draw(SceneInfo &scene_info) override;
    virtual void post_draw() override;
    virtual void draw(Mesh *mesh, SceneInfo &scene_info,
                      glm::mat4 to_world = glm::mat4(1.f)) override;

    GLuint FBO, rbo_depth;
    GLuint color_buffers[2]; // 1 for hdr, 1 for bloom
    int screen_width = 1280;
    int screen_height = 720; // these hard-coded values will be overwritten by window resize events

private:
    GLuint VAO, VBO; // for quad rendering
    bool hdr_on = true;
    float hdr_exposure = 1.f;
    void bind_buffers();
};