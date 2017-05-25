#pragma once

#include "trap.h"

/* 
    Traps that move and attack by using their collider box
    Currently, will change direction when it hits the wall.
    Later, we want to split differnet move types for WALL, TIME, and AI. 
*/
class MobileTrap : public Trap {
private:
    btBoxShape *hit_box = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
protected:
    float move_speed;
    virtual void handle_movement();
    virtual void change_direction();

public:
    MobileTrap(int x, int z, int id = 0);

    virtual void s_update_this() override;

    virtual void setup_model() override {};

    virtual void s_on_collision(GameObject *other) override;
    virtual void c_on_collision(GameObject *other) override;
};