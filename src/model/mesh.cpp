#include "mesh.h"
#include "geometry.h"
#include "shaders/shader_manager.h"

Mesh::Mesh()
{
	this->shader = ShaderManager::get_default();
	this->to_world = glm::mat4(1.0f);
	this->no_culling = false;
}

Mesh::Mesh(Geometry *geo, Material *mat)
{
	this->geometry = geo;
	this->material = mat;
	this->shader = ShaderManager::get_default();
	this->to_world = glm::mat4(1.0f);
	this->no_culling = false;
}

// Constructor with default values for world matrix and no_culling.
Mesh::Mesh(Geometry *geo, Material *mat, Shader *shader)
{
	this->geometry = geo;
	this->material = mat;
	this->shader = shader;
	this->to_world = glm::mat4(1.0f);
	this->no_culling = false;
}

Mesh::Mesh(Geometry *geo, Material *mat, Shader *shader, glm::mat4 m, bool no_cull)
{
	this->geometry = geo;
	this->material = mat;
	this->shader = shader;
	this->to_world = m;
	this->no_culling = no_cull;
}

Mesh::~Mesh()
{
}
