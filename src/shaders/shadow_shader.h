#pragma once
#include "shader.h"

#include <glm/glm.hpp>

class ShadowShader :
	public Shader
{
public:
	GLuint FBO, shadow_map_tex;
	unsigned int size;
	glm::mat4 light_matrix;
	glm::vec3 light_pos;
	glm::mat4 light_proj;

	ShadowShader(GLuint shader_id);
	void set_material(Material m);
	void draw(Geometry *g, glm::mat4 to_world);
};

