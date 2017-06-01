#pragma once

/*
* This class will be used a model-less collider in
* the various minigames. Will usually act as the off-stage
* collider.
*/

#include "../game_object.h"
#include "btBulletDynamicsCommon.h"
#include "events.h"

class EmptyCollider : public GameObject {
private:
    btBoxShape *hit_box = new btBoxShape(btVector3(20, 20, 10));
public:
    EmptyCollider(int id = 0);

    void s_on_collision(GameObject *other) override;
};