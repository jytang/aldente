#pragma once

#include "trap.h"

/* 
    Traps that move and attack by using their collider box
    Currently, will change direction when it hits the wall.
    Later, we want to split differnet move types for WALL, TIME, and AI. 
*/
class MobileTrap : public Trap {
private:
    std::function<void()> cancel_timer;
protected:
    float move_speed = 1.0f;
    float rotation_amount; // Amount to rotate when changing directions (Counterclockwise)
    bool random_rotations_on; // Whether or not should use random rotations
    
    // Designated which type of movement monster uses
    enum MoveType {
        WALL, TIME, AI
    };

    MoveType move_type;

    virtual void change_direction();

    void handle_movement();    

    // Checks, based on Mobile Trap type.
    void check_wall(); // Changes direction when it hits a wall
    void setup_timer(long long time_interval_ms); // Changes direction every period
    virtual void update_ai() {};   // Checks for player every frame and changes direction if needed

    btBoxShape *hit_box = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
    btBoxShape *hit_box_small = new btBoxShape(btVector3(0.3f, 0.3f, 0.3f));

public:
    MobileTrap(int x, int z, int id = 0);

    virtual void s_update_this() override;
};