#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "scene_camera.h"
#include "scene_group.h"
#include "shaders/shader.h"
#include "model/plane.h"

/*
	Contains all the information for a single scene
*/
class Scene {
public:
    SceneGroup *root;
    SceneCamera *camera;
    glm::vec3 light_pos;

    Scene();

    virtual ~Scene();

    void draw();

    void pass(Shader *s);

    virtual void update();

    virtual void setup() = 0;
};
