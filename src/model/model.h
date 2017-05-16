#pragma once

/*
    A Model is made up of a bunch of meshes to form a 3D renderable object.
*/

#include <vector>

#include "shaders/shader_manager.h"
#include "shaders/shader.h"
#include "mesh.h"
#include "scene/scene_info.h"
#include "skeleton.h"

class Model {
private:
    Shader *model_shader;
    std::vector<glm::mat4> bones; // Final transformation matrix for bones, including animation

public:
    std::vector<Mesh *> meshes;
    Skeleton initial_skeleton; // Initial skeleton that model is compatible with, not updated.

    Model(Shader *shader = &ShaderManager::basic) : model_shader(shader) {}

    void draw(Shader *shader, SceneInfo &scene_info, glm::mat4 to_world);

    void add_mesh(Mesh *m);

    void set_color(Color color);

    void set_alpha(float alpha);

    void set_shadows(bool enable);

    void set_bones(Skeleton *skel) { bones = skel->bones_final; };

    void set_shader(Shader *shader) { model_shader = shader; }
};

