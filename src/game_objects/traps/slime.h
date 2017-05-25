#pragma once

#include "mobile_trap.h"

class Slime : public MobileTrap {
public:
    Slime(int x, int z, int id = 0);

    void setup_model() override;
};