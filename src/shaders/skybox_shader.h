#pragma once

#include "shader.h"

#include <vector>
#include <string>

class SkyboxShader : public Shader {
private:
    Geometry *cube_geometry;
    GLuint current_texture_id;
    std::map<std::string, GLuint> texture_ids;
public:
    virtual void init() override;
    virtual void pre_draw(SceneInfo &scene_info) override;
    virtual void post_draw() override;
    virtual void draw(Mesh *mesh, SceneInfo &scene_info,
                      glm::mat4 to_world = glm::mat4(1.f)) override;

    void set_skybox(std::string skybox_name);
    static std::vector<std::string> skyboxes;

    glm::mat4 rot; // random rotation to skybox
    float rot_angle;
    glm::vec3 rot_axis;
};

