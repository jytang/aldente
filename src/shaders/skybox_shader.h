#pragma once

#include "shader.h"

class SkyboxShader : public Shader {
public:
    virtual void init() override;
    virtual void pre_draw(SceneInfo &scene_info) override;
    virtual void post_draw();
    virtual void draw(Mesh &mesh, SceneInfo &scene_info,
                      glm::mat4 to_world) override;





    GLuint current_texture_id = 0;
    std::vector<GLuint> texture_ids;
    GLuint VAO, VBO;

    SkyboxShader(GLuint shader_id);

    void load_cubemap();

    void set_material(Material *m);

    void draw(Geometry *g, glm::mat4 to_world);
};

