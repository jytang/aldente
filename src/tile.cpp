#include "tile.h"
#include "util/colors.h"

Tile::Tile() {}

Tile::~Tile() {}

FloorTile::~FloorTile() {}

WallTile::~WallTile() {}

FloorTile::FloorTile(int x, int z) {
    width = 1;
    height = 1;
    this->x = x;
    this->z = z;
    mesh = new Mesh();

    mesh->to_world = glm::mat4(1.0f);
    mesh->geometry = GeometryGenerator::generate_plane(0.5f, 0);
    mesh->shader = ShaderManager::get_default();
    Material *mat = new Material(color::indian_red);
    mesh->material = mat;

    btDefaultMotionState *motionstate = new btDefaultMotionState(btTransform(
            btQuaternion(), btVector3((btScalar) x, 0.0f, (btScalar) z)));

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
            0,                  // mass, in kg. 0 -> Static object, will never move.
            motionstate,
            ground,  // collision shape of body
            btVector3(0, 0, 0)    // local inertia
    );
    rigidBody = new btRigidBody(rigidBodyCI);

    // Will be used to know which object is picked.
    rigidBody->setUserPointer(this);
}

void FloorTile::update(Tile *hover) {
    btTransform t;

    // Get the transform from Bullet and into 't'
    rigidBody->getMotionState()->getWorldTransform(t);
    mesh->to_world[3] = glm::vec4((float) t.getOrigin().getX(), (float) t.getOrigin().getY(),
                                  (float) t.getOrigin().getZ(), 1.0f);
    if (hover == this) {
        Material *mat = new Material(color::windwaker_green);
        mesh->material = mat;
    } else {
        Material *mat = new Material(color::indian_red);
        mesh->material = mat;
    }
    //fprintf(stderr, "%f\n", mesh->to_world[3].y);
    //mesh->to_world *= glm::translate(glm::mat4(1.f), glm::vec3(0, 5.0f, 0));
    // Convert the btTransform into the GLM matrix using 'glm::value_ptr'
    /*t.getOpenGLMatrix(glm::value_ptr(mesh->to_world));
    mesh->to_world = glm::transpose(mesh->to_world);
    fprintf(stderr, "%f\n", t.getOrigin().getY());*/
}

WallTile::WallTile(int x, int z) {
    width = 1;
    height = 1;
    this->x = x;
    this->z = z;
    mesh = new Mesh();

    mesh->to_world = glm::mat4(1.0f);
    //mesh->to_world *= glm::translate(glm::mat4(1.f), glm::vec3(0,0,0));
    mesh->geometry = GeometryGenerator::generate_cube(1.0f, true);
    mesh->shader = ShaderManager::get_default();
    Material *mat = new Material(color::indian_red);
    mesh->material = mat;

    btDefaultMotionState *motionstate = new btDefaultMotionState(btTransform(
            btQuaternion(), btVector3((btScalar) x, 0.5f, (btScalar) z)));

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
            0,                  // mass, in kg. 0 -> Static object, will never move.
            motionstate,
            box,  // collision shape of body
            btVector3(0, 0, 0)    // local inertia
    );
    rigidBody = new btRigidBody(rigidBodyCI);

    // Will be used to know which object is picked.
    rigidBody->setUserPointer(this);
}

void WallTile::update(Tile *hover) {
    btTransform t;

    // Get the transform from Bullet and into 't'
    rigidBody->getMotionState()->getWorldTransform(t);
    mesh->to_world[3] = glm::vec4((float) t.getOrigin().getX(), (float) t.getOrigin().getY(),
                                  (float) t.getOrigin().getZ(), 1.0f);
    if (hover == this) {
        Material *mat = new Material(color::windwaker_green);
        mesh->material = mat;
    } else {
        Material *mat = new Material(color::indian_red);
        mesh->material = mat;
    }
    //fprintf(stderr, "%f\n", mesh->to_world[3].y);
    //mesh->to_world *= glm::translate(glm::mat4(1.f), glm::vec3(0, 5.0f, 0));
    // Convert the btTransform into the GLM matrix using 'glm::value_ptr'
    /*t.getOpenGLMatrix(glm::value_ptr(mesh->to_world));
    mesh->to_world = glm::transpose(mesh->to_world);
    fprintf(stderr, "%f\n", t.getOrigin().getY());*/
}
