#include "trap.h"

class Spikes : public Trap {
public:
    Spikes(int x, int z, int id = 0);

    void setup_model() override;
    void s_on_collision(GameObject *other) override;
    void c_on_collision(GameObject *other) override;

private:
    btBoxShape *hit_box = new btBoxShape(btVector3(0.25f, 0.25f, 0.25f));
};
