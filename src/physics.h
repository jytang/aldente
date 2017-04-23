#pragma once

#include <glm/glm.hpp>

#include "btBulletDynamicsCommon.h"
#include "tile.h"
#include <vector>

class Physics {
private:
    Physics();

    //Bullet varaibles
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
public:
    static Physics *physics; // Singleton

    ~Physics();

    btDiscreteDynamicsWorld *dynamicsWorld;
    std::vector<btRigidBody *> rigidBodies;
    Tile *hover;

    void setup_bullet();

    void raycast_mouse(double xpos, double ypos, int width, int height);

    void update();
};

