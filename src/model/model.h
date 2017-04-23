#pragma once

/*
	A Model is made up of a bunch of meshes to form a 3D renderable object.
*/

#include <vector>

#include "scene/scene.h"
#include "scene/scene_node.h"
#include "shaders/shader.h"
#include "mesh.h"

class Model :
        public SceneNode {
public:
    std::vector<Mesh *> meshes;
    glm::mat4 model_mat;

    Model();

    Model(Scene *);

    ~Model();

    void set_scene(Scene *scene);

    void add_mesh(Mesh *m);

    void combine_meshes();

    void draw();

    void update();

    void pass(Shader *s);
};

