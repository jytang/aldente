#pragma once

/*
* Penguin Class:
* This class will contain all functionality concerning a penguin
* in the "Pushy Penguins" minigame.
*/

#include "../game_object.h"
#include "btBulletDynamicsCommon.h"
#include "events.h"
#include "game/direction.h"

class Penguin : public GameObject {
private:
    float move_speed;
    Direction dir;

    btCapsuleShape *hit_capsule = new btCapsuleShape(0.5f, 0.2f);
public:
    Penguin(int id = 0);

    void s_update_this() override;
    void c_update_state(glm::mat4 mat, bool enab) override;

    void setup_model() override;

    // Sets the penguin's position to the spawn area.
    void reset_position();

    void set_speed(float to_set) { move_speed = to_set; };

    // Set permanent movement in dir direction (only left for now)
    void set_movement(Direction to_set);

    void do_movement();
};
