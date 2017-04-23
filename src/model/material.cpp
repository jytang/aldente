#include "material.h"

// Constructor with default values
Material::Material() {
    ambient = {0.f, 0.f, 0.f};
    diffuse = {1.f, 1.f, 1.f};
    specular = {0.f, 0.f, 0.f};
    shininess = 50.f;
    shadows = true;
}

Material::Material(glm::vec3 color) {
    ambient = color;
    diffuse = color;
    specular = {0.f, 0.f, 0.f};
    shininess = 50.f;
    shadows = true;
}

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat shininess, bool shadows) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->shininess = shininess;
    this->shadows = shadows;
}