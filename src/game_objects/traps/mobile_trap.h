#pragma once

#include "trap.h"

/* 
    Traps that move and attack by using their collider box
*/
class MobileTrap : public Trap {
private:
    btBoxShape *hit_box = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
protected:
    // Movement Type for Mobile Trap.
    enum MoveType {
        WALL, // Changes direction on collision with a wall
        TIME, // Changes direction every time interval
        AI    // Uses AI for tracking a target
    };

    MoveType move_type;

    glm::vec2 turn_vector; // Direction to turn based on WALL or TIME move type.

    virtual void handle_movement();

public:
    MobileTrap(int x, int z, int id = 0);

    virtual void s_update_this() override;
    virtual void c_update_state(float x, float z, float wx, float wz, bool enab) override;

    virtual void setup_model() override {};

    virtual void s_on_collision(GameObject *other) override;
    virtual void c_on_collision(GameObject *other) override;
};