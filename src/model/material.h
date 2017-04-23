#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

/*
	The overall color of the model, as well as how it is
	affected by the lighting in the scene.
*/
class Material {
public:
    Material();

    Material(glm::vec3 color);

    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat shininess, bool shadows);

    ~Material();

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLfloat shininess;
    bool shadows;
};

