#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

struct Material
{
	glm::vec3 ambient = { 0.f, 0.f, 0.f };
	glm::vec3 diffuse = { 1.f, 1.f, 1.f };
	glm::vec3 specular = { 0.f, 0.f, 0.f };
	GLfloat shininess = 50.f;
	bool shadows = true;
};

