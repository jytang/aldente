#pragma once

/*
    A Model is made up of a bunch of meshes to form a 3D renderable object.
*/

#include <vector>

#include "shaders/shader.h"
#include "mesh.h"
#include "scene/scene.h"

class Model {
private:
    std::vector<Mesh *> meshes;

    Shader *model_shader;

public:
    Model(Shader *shader = &Shader::basic) : model_shader(shader) {}

    void add_mesh(Mesh *m);

    void draw(Shader &shader, SceneInfo &scene_info, glm::mat4 to_world);
};

