#include "collision_trap.h"

class Spikes : public CollisionTrap {
public:
    Spikes(int x, int z, int id = 0);

    void setup_model() override;

    void play_trigger_animation() override;

private:
    btBoxShape *hit_box = new btBoxShape(btVector3(0.25f, 0.25f, 0.25f));
};
