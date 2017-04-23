#pragma once

#include <GL/glew.h>
#include "model/geometry.h"
#include "material.h"
#include "shaders/shader.h"
#include "shaders/shader_manager.h"

#include <glm/glm.hpp>
#include <map>
#include <assimp/scene.h>

struct Anim {
    std::vector<unsigned int> bones;
    std::vector<float> weight;
};

class Mesh {
public:
    Mesh(Geometry *geo = NULL,
         Material *mat = NULL,
         Shader *shader = ShaderManager::get_default(),
         glm::mat4 m = glm::mat4(1.0f),
         bool no_cull = false);
    //~Mesh();

    Geometry *geometry;
    Material *material;
    Shader *shader;
    glm::mat4 to_world;
    bool no_culling;
    // map<string, Anim*> animations();
    aiMatrix4x4 inverseBoneMat;
};
