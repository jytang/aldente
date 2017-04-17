#pragma once
using namespace std;

#include <GL/glew.h>
#include "geometry/geometry.h"
#include "material.h"
#include "shaders/shader.h"
#include <glm/glm.hpp>

struct Mesh
{
	Geometry *geometry;
	Material material;
	Shader *shader;
	glm::mat4 to_world;
	bool no_culling;
};
