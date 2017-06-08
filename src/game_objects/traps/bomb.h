#pragma once

#include "trap.h"

class Bomb : public Trap {
public:
    Bomb(int x, int z, int id = 0);

    void s_on_collision(GameObject *other) override;
    void c_on_collision(GameObject *other) override;

    void s_reset() override;
    void c_reset() override;

    void setup_model() override;
private:
    float blast_radius;
    bool exploded;

    btBoxShape *hit_box = new btBoxShape(btVector3(0.25f, 0.25f, 0.25f));

    void s_explode();
    void c_explode();
};
