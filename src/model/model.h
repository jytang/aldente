#pragma once

/*
    A Model is made up of a bunch of meshes to form a 3D renderable object.
*/

#include <vector>

#include "shaders/shader_manager.h"
#include "shaders/shader.h"
#include "mesh.h"
#include "scene/scene_info.h"

#include "animation/animation.h"

class Model {
private:
    Shader *model_shader;

public:
    std::vector<Mesh *> meshes;
    std::map<std::string, Animation *> animations;

    // Skeleton Information
    glm::mat4 global_inv_trans; // Global Inverse Transform, Used for calculating bones_final
    std::map<std::string, GLuint> bone_mapping; // Maps bone name to index for easy access of bones
    std::vector<glm::mat4> bone_offsets; // Initial offset matrix for bone, used for calculating bones_final
    std::vector<glm::mat4> bones_default; // Defaults matrices for bones to be in default pose without animations
    std::vector<glm::mat4> bones_final; // Final transformation matrix for bone, including animation    

    Model(Shader *shader = &ShaderManager::basic) : model_shader(shader) {}

    void set_shader(Shader *shader) { model_shader = shader; }

    void add_mesh(Mesh *m);

    void draw(Shader *shader, SceneInfo &scene_info, glm::mat4 to_world);
};

