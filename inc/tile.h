/*
* Tile Class:
*   This file holds the superclass of Tile and all of its subclasses. The superclass Tile holds basic
*   varaibles and functions that all Tiles will require in order to function. These include, and are not
*   limited to: width, height, x coord, z coord, Mesh, getter functions. Each actual Tile will then 
*   be a child of this superclass and implement the neccessary functionality for that specific tile
*   (for example: rendering, collision detection, trigger functionality).
*/


#pragma once

#include "mesh.h"
#include "geometry_generator.h"
#include "shader_manager.h"
#include "colors.h"
#include "btBulletDynamicsCommon.h"

class Tile
{
protected:
	int width, height, x, z;
	bool traversable;
	Mesh* mesh;
	btRigidBody* rigidBody;

private:

public:
	Tile();
	~Tile();

	Mesh* getMesh() { return mesh; };
	int getX() { return x; };
	int getZ() { return z; };
	virtual void update() {};
	btRigidBody* getRigid() { return rigidBody; };
};

class FloorTile : public Tile
{

public:
	FloorTile(int x, int z);
	~FloorTile();

	
};

class WallTile : public Tile
{

public:
	WallTile(int x, int z);
	~WallTile();

	void update();
};
