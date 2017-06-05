#pragma once

/*
* Platform class: For all your platform needs
*/

#include "../game_object.h"
#include "btBulletDynamicsCommon.h"

class Platform : public GameObject {
private:
    btBoxShape *hit_box = new btBoxShape(btVector3(10, 0.5f, 5));
public:
    Platform();

    void setup_model() override;
};
