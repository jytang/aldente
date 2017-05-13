#pragma once

/*
* Player Class:
* This class will contain all functionality concerning a player 
* gameobject such as health, gold, movement, etc.
*/

#include "game_object.h"
#include "game/direction.h"
#include "btBulletDynamicsCommon.h"
#include "construct.h"
#include "events.h"

class Player : public GameObject {
private:

    // to_move is for saving the joystick input in each frame.
    int to_moveX;
    int to_moveZ;

    float move_speed;

    btCapsuleShape *hit_capsule = new btCapsuleShape(0.5f, 1.0f);
public:
    Player();
    Player(int obj_id);

    virtual void update_this() override;
    virtual void on_collision(GameObject *other) override;
    virtual void on_collision_graphical() override;

    void prepare_movement(int inX, int inZ);
    void update_state(float x, float z, float wx, float wz, bool camera);
    void do_movement();
    void setup_listeners();

    // Called when the player tries to interacts with an object (e.g pressing A)
    void interact();

    void start_walk();
    void stop_walk();

    // TODO after we make more player models
    void setup_model() {}
};