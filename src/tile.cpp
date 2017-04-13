#include "tile.h"

Tile::Tile() {}
Tile::~Tile() {}

FloorTile::FloorTile(int x, int z) {
	width = 1;
	height = 1;
	this->x = x;
	this->z = z;
	mesh = Mesh();

	mesh.to_world = glm::mat4(1.0f);
	mesh.geometry = GeometryGenerator::generate_plane(0.5f, STONE);
	mesh.shader = ShaderManager::get_default();
	Material mat;
	mat.diffuse = mat.ambient = color::indian_red;
}

WallTile::WallTile(int x, int z) {
	width = 1;
	height = 1;
	this->x = x;
	this->z = z;
	mesh = Mesh();

	mesh.to_world = glm::mat4(1.0f);
	mesh.to_world *= glm::translate(glm::mat4(1.f), glm::vec3(0,0.5f,0));
	mesh.geometry = GeometryGenerator::generate_cube(1.0f, true);
	mesh.shader = ShaderManager::get_default();
	Material mat;
	mat.diffuse = mat.ambient = color::indian_red;
}