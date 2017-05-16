#pragma once

#include <glm/glm.hpp>
#include "game_object.h"
#include "btBulletDynamicsCommon.h"
#include "events.h"

// Note: Opengl has the x and z axis as its ground plane

class Construct : public GameObject {
protected:
    btBoxShape *hit_box = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
    btCylinderShape *goal_hit_box = new btCylinderShape(btVector3(0.005f, 0.5f, 0.005f));
public:
    Construct(int x, int z);
    Construct(int x, int z, int id);

    virtual void setup_model() = 0;
    virtual void update_this() override = 0;
};

class Chest : public Construct {
public:
    Chest(int x, int z);
    Chest(int x, int z, int id);
    void interact_trigger();
    void setup_model() override;
    void update_this() override;
};

class Goal : public Construct {
public:
    Goal(int x, int z);
    Goal(int x, int z, int id);
    void setup_model();
    void on_collision(GameObject *other);
    void on_collision_graphical();
    void update_this();
};
