#pragma once

/*
* Penguin Class:
* This class will contain all functionality concerning a penguin
* in the "Pushy Penguins" minigame.
*/

#include "../game_object.h"
#include "btBulletDynamicsCommon.h"
#include "events.h"

class Penguin : public GameObject {
private:
    float move_speed;
    //int row;

    btCapsuleShape *hit_capsule = new btCapsuleShape(0.5f, 0.2f);
public:
    Penguin(int id = 0);

    void s_update_this() override;
    void c_update_state(glm::mat4 mat, bool enab) override;

    void setup_model() override;

    // Sets the penguin's position to the spawn area.
    void reset_position();
};
