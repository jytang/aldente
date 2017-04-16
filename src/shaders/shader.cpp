#include "shader.h"

#include <iostream>

Shader::Shader(GLuint shader_id)
    : shader_id(shader_id) {}

void Shader::use()
{
    glUseProgram(shader_id);
}

void Shader::send_cam_pos(glm::vec3 cam_pos)
{
    this->cam_pos = cam_pos;
}

void Shader::set_VP(glm::mat4 V, glm::mat4 P)
{
    this->V = V;
    this->P = P;
}

void Shader::send_mesh_model(glm::mat4 mesh_model)
{
    this->mesh_model = mesh_model;
}

void Shader::set_material(Material m) {}

void Shader::draw(Geometry *g, glm::mat4 to_world) {}
