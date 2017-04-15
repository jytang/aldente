#include "tile.h"

Tile::Tile() {}
Tile::~Tile() {}

FloorTile::FloorTile(int x, int z) {
	width = 1;
	height = 1;
	this->x = x;
	this->z = z;
	mesh = new Mesh();

	mesh->to_world = glm::mat4(1.0f);
	mesh->geometry = GeometryGenerator::generate_plane(0.5f, STONE);
	mesh->shader = ShaderManager::get_default();
	Material mat;
	mat.diffuse = mat.ambient = color::indian_red;
	mesh->material = mat;
}

WallTile::WallTile(int x, int z) {
	width = 1;
	height = 1;
	this->x = x;
	this->z = z;
	mesh = new Mesh();

	mesh->to_world = glm::mat4(1.0f);
	mesh->to_world *= glm::translate(glm::mat4(1.f), glm::vec3(0,0.5f,0));
	mesh->geometry = GeometryGenerator::generate_cube(1.0f, true);
	mesh->shader = ShaderManager::get_default();
	Material mat;
	mat.diffuse = mat.ambient = color::indian_red;
	mesh->material = mat;
	
	btBoxShape* box = new btBoxShape(btVector3(1, 1, 1));

	btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(
		btQuaternion(), btVector3(0, 0.5f, 0)));

	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		1,                  // mass, in kg. 0 -> Static object, will never move.
		motionstate,
		box,  // collision shape of body
		btVector3(0, 0, 0)    // local inertia
	);
	rigidBody = new btRigidBody(rigidBodyCI);

	// Will be used to know which object is picked. 
	//rigidBody->setUserPointer(this);
}

void WallTile::update() {
	btTransform t;

	// Get the transform from Bullet and into 't'
	rigidBody->getMotionState()->getWorldTransform(t);
	mesh->to_world[3] = glm::vec4((float)t.getOrigin().getX(), (float)t.getOrigin().getY(), (float)t.getOrigin().getZ(),1.0f);
	//fprintf(stderr, "%f\n", mesh->to_world[3].y);
	//mesh->to_world *= glm::translate(glm::mat4(1.f), glm::vec3(0, 5.0f, 0));
	// Convert the btTransform into the GLM matrix using 'glm::value_ptr'
	/*t.getOpenGLMatrix(glm::value_ptr(mesh->to_world));
	mesh->to_world = glm::transpose(mesh->to_world);
	fprintf(stderr, "%f\n", t.getOrigin().getY());*/
}