#pragma once

#include "mobile_trap.h"

class Slime : public MobileTrap {
public:
    Slime(int x, int z, int id = 0);

    virtual void setup_model() override {};

    virtual void s_interact_trigger(GameObject *other) override {};
    virtual void c_interact_trigger(GameObject *other) override {};
    virtual void s_on_collision(GameObject *other) override {};
    virtual void c_on_collision(GameObject *other) override {};
};