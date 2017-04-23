#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "model/material.h"
#include "model/geometry.h"

class Shader {
public:
    GLuint shader_id;
    glm::mat4 V, P, mesh_model;
    glm::vec3 cam_pos;

    Shader(GLuint shader_id);

    void use();

    void send_cam_pos(glm::vec3 cam_pos);

    void set_VP(glm::mat4 V, glm::mat4 P);

    void send_mesh_model(glm::mat4 mesh_model);

    void set_culling(bool b);

    virtual void set_material(Material *m);

    virtual void draw(Geometry *g, glm::mat4 to_world);
};
