#pragma once

#include "collision_trap.h"

/* 
    Traps that move and attack by using their collider box.
    There are three movement types for mobile traps:
        Wall - Changes direction whenever it hits a wall
        Time - Changes direction every certain interval
        AI - Not yet defined. Define it in ur own class.

    All mobile traps always move forward, and currently do not stop.
*/
class MobileTrap : public CollisionTrap {
private:
    std::function<void()> cancel_timer;
protected:
    float move_speed = 1.0f;
    float rotation_amount = 90.0f; // Amount to rotate when changing directions (Counterclockwise)
    bool random_rotations_on = false; // Whether or not should use random rotations

    bool stop_moving = false; // Tells mobile trap to stop moving
    
    // Designated which type of movement monster uses
    enum MoveType {
        WALL, TIME, AI
    };

    MoveType move_type = WALL;

    virtual void change_direction();

    void handle_movement();    

    // Checks, based on Mobile Trap type.
    void check_wall(); // Changes direction when it hits a wall
    void setup_timer(long long time_interval_ms); // Changes direction every period
    virtual void update_ai() {};   // Checks for player every frame and changes direction if needed

    btBoxShape *hit_box = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
    btBoxShape *hit_box_small = new btBoxShape(btVector3(0.3f, 0.5f, 0.3f));
    btCapsuleShape *hit_capsule = new btCapsuleShape(0.3f, 1.0f);

public:
    MobileTrap(int x, int z, int id = 0);

    virtual void s_update_this() override;

    virtual void s_reset() override;
    virtual void c_reset() override;
};