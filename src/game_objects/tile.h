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

#include <string>

class Tile : public GameObject {
protected:
    int x, z;

    Construct *construct;
    btBoxShape *hit_box = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
    btBoxShape *hit_plane = new btBoxShape(btVector3(0.5f, 0.05f, 0.5f));
public:
    Tile();

    // Whether a construct buildable on this tile.
    bool buildable;
    bool traversable;

    int getX() { return x; };
    int getZ() { return z; };

    virtual void setup_instanced_model(int num_instances,
                                       std::vector<glm::mat4> instance_matrix) override = 0;

    void set_construct(Construct* to_set) { construct = to_set; };
    Construct* get_construct() { return construct; };

    bool isBuildable() { return buildable; };
};

// Grass Tile
class FloorTile : public Tile {
public:
    FloorTile(int x, int z);

    void setup_instanced_model(int num_instances,
                               std::vector<glm::mat4> instance_matrix) override;
};

// Tile that contains no model, only used to hold tile information
class EmptyTile : public Tile {
protected:
    std::string model_name;
public:
    EmptyTile(int x, int z);

    void setup_instanced_model(int num_instances,
        std::vector<glm::mat4> instance_matrix) override;
};

// Tree Tile, replacing the old Wall Tile
class TreeTile : public Tile {
public:
    TreeTile(int x, int z);

    void setup_instanced_model(int num_instances,
                               std::vector<glm::mat4> instance_matrix) override;
};

/***** Environment Objects *****/

class BigRockTile : public EmptyTile {
public:
    BigRockTile(int x, int z) : EmptyTile(x, z) { model_name = "rock_big"; };
};

class SmallRockTile : public EmptyTile {
public:
    SmallRockTile(int x, int z) : EmptyTile(x, z) { model_name = "rock_small"; };
};

class GrassTile : public EmptyTile {
public:
    GrassTile(int x, int z) : EmptyTile(x, z) { model_name = "grass"; };
};