#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "util/colors.h"

/*
    The overall color of the model, as well as how it is
    affected by the lighting in the scene.
*/
class Material {
public:
    Material(glm::vec3 diffuse = color::white,
            glm::vec3 ambient = color::black,
            glm::vec3 specular = color::black,
            GLfloat shininess = 50.f, bool shadows = true)
        : ambient(ambient), diffuse(diffuse), specular(specular),
          shininess(shininess), shadows(shadows) {}

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLfloat shininess;
    bool shadows;
};

