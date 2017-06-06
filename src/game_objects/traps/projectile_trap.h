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
    std::vector<Projectile *> projectile_pool; // Poolable amount of projectiles to grab objects from

    // How the trap will be activated
    enum ActivationType {
        TRIGGER, TIME, RAYCAST, AI
    };

    ActivationType activation_type = TIME;
    float raycast_range = 5.0f;

    virtual void update_ai() {};
    void raycast_check();
    void setup_timer(long long time_interval_ms);

    // Makes copies of the given projectile to fill the projectil_pool with
    void fill_projectile_pool(ProjectileTypes type);

    // Fires game object
    void shoot();

    virtual void play_trigger_animation() {};
public:
    ProjectileTrap(int x, int z, int id = 0);

  
  virtual void s_update_this() override;

  void c_on_collision(GameObject* other) override;
};