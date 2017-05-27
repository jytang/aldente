#pragma once

#include "trap.h"
#include "projectile.h"

/*
    Traps that spawn another game object and uses its child's collider to
    damage the player.
*/
class ProjectileTrap : public Trap {
private:
    std::function<void()> cancel_timer;
protected:
    Projectile *projectile;

    // How the trap will be activated
    enum ActivationType {
        TRIGGER, TIME, RAYCAST,
    };

    ActivationType activation_type = TIME;

    //void raycast_check();
    void setup_timer(long long time_interval_ms);

    void attach_projectile(Projectile *projectile);

    // Fires game object
    void shoot();

public:
    ProjectileTrap(int x, int z, int id = 0);
};