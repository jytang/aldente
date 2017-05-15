/*
* Tile Class:
*   This file holds the superclass of Tile and all of its subclasses. The superclass Tile holds basic
*   varaibles and functions that all Tiles will require in order to function. These include, and are not
*   limited to: width, height, x coord, z coord, Mesh, getter functions. Each actual Tile will then
*   be a child of this superclass and implement the neccessary functionality for that specific tile
*   (for example: rendering, collision detection, trigger functionality).
*/

#pragma once

#include "model/mesh.h"
#include "model/material.h"
#include "model/geometry_generator.h"
#include "btBulletDynamicsCommon.h"
#include "game_object.h"
#include "construct.h"
#include "events.h"

class Tile : public GameObject {
protected:
    int width, height, x, z;
    bool traversable;

    Construct *construct;
    btBoxShape *hit_box = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
    btBoxShape *hit_plane = new btBoxShape(btVector3(0.5f, 0.05f, 0.5f));
private:

public:
    Tile();

    // Whether a construct buildable on this tile.
    bool buildable;

    int getX() { return x; };
    int getZ() { return z; };

    virtual void update_this() override = 0;
    virtual void setup_instanced_model(int num_instances,
                                       std::vector<glm::mat4> instance_matrix) override = 0;

    void set_construct(Construct* to_set) { construct = to_set; };
    Construct* get_construct() { return construct; };

    bool isBuildable() { return buildable; };
};

class FloorTile : public Tile {

public:
    FloorTile(int x, int z);

    void setup_instanced_model(int num_instances,
                               std::vector<glm::mat4> instance_matrix) override;
    void update_this() override{};
};

class WallTile : public Tile {

public:
    WallTile(int x, int z);

    void setup_instanced_model(int num_instances,
                               std::vector<glm::mat4> instance_matrix) override;
    void update_this() override{};
};
