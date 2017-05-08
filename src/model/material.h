#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "util/color.h"

/*
    The overall color of the model, as well as how it is
    affected by the lighting in the scene.
*/
class Material {
public:
    Material(Color diffuse = Color::WHITE,
            Color ambient = Color::WHITE,
            Color specular = Color::WHITE,
            GLfloat shininess = 50.f, bool shadows = true)
        : ambient(ambient), diffuse(diffuse), specular(specular),
          shininess(shininess), shadows(shadows) {}

    Color ambient;
    Color diffuse;
    Color specular;
    GLfloat shininess;
    bool shadows;
};