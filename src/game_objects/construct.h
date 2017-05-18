#pragma once

#include <glm/glm.hpp>
#include "game_object.h"
#include "btBulletDynamicsCommon.h"
#include "events.h"

// Note: Opengl has the x and z axis as its ground plane

class Construct : public GameObject {
public:
    Construct(int x = 0, int z = 0);
    Construct(int x, int z, int id);

    virtual void setup_model() override {};

    virtual void s_update_this() override;

    virtual void s_interact_trigger(GameObject *other) override {};
    virtual void c_interact_trigger() override {};
    virtual void s_on_collision(GameObject *other) override {};
    virtual void c_on_collision() override {};
};

class Chest : public Construct {
public:
    Chest(int x = 0, int z = 0);
    Chest(int x, int z, int id);

    void setup_model() override;
    
    void s_interact_trigger(GameObject *other) override;
    void c_interact_trigger() override;

private:
    btBoxShape *hit_box = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
};

class Goal : public Construct {
public:
    Goal(int x = 0, int z = 0);
    Goal(int x, int z, int id);

    void setup_model() override;

    void s_on_collision(GameObject *other) override;
    void c_on_collision() override;

private:
    btCylinderShape *goal_hit_box = new btCylinderShape(btVector3(0.005f, 0.5f, 0.005f));
};

class Spikes : public Construct {
public:
    Spikes(int x = 0, int z = 0);
    Spikes(int x, int z, int id);

    void setup_model() override;
    void s_on_collision(GameObject *other) override;
    void c_on_collision() override;

private:
    btBoxShape *hit_box = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
};
