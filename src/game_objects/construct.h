#pragma once

#include <glm/glm.hpp>
#include "game_object.h"
#include "btBulletDynamicsCommon.h"
#include "events.h"

// Note: Opengl has the x and z axis as its ground plane

class Construct : public GameObject {
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
    void on_interact(GameObject *other) override;
    void setup_model() override;
    void update_this() override;
private:
    btBoxShape *hit_box = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
};

class Goal : public Construct {
public:
    Goal(int x, int z);
    Goal(int x, int z, int id);
    void setup_model();
    void on_collision(GameObject *other);
    void on_collision_graphical(GameObject *other) override;
    void update_this();
private:
    btCylinderShape *goal_hit_box = new btCylinderShape(btVector3(0.005f, 0.5f, 0.005f));
};
