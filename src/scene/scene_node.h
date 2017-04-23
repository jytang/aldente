#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shaders/shader.h"

class Scene;

class SceneNode {
public:
    virtual ~SceneNode() {};

    virtual void draw() = 0;

    virtual void update() = 0;

    virtual void pass(Shader *s) = 0;

protected:
    Scene *scene;
};

