#pragma once

#include <glm/glm.hpp>
#include "game_object.h"
#include "btBulletDynamicsCommon.h"
#include "events.h"

// Note: Opengl has the x and z axis as its ground plane

class Construct : public GameObject {
protected:
    btBoxShape *hit_box = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
public:
    Construct(int x, int z);

    // Callback for when a player presses A on this construct
    virtual void interact_trigger() = 0;

    virtual void setup_model() = 0;
    virtual void update() override = 0;
};

class Crate : public Construct {
public:
    Crate(int x, int z);
    void setup_model();
    void update() override;
    void interact_trigger() override;
};
