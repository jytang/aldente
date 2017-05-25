#pragma once

#include "mobile_trap.h"

class Slime : public MobileTrap {
public:
    Slime(int x, int z, int id = 0);

    void setup_model() override;
};

class SlimeBlue : public Slime {
public:
    SlimeBlue(int x, int z, int id = 0);
};

class SlimeYellow : public Slime {
public:
    SlimeYellow(int x, int z, int id = 0);
};

class SlimeRed : public Slime {
public:
    SlimeRed(int x, int z, int id = 0);
};

class SlimeGreen : public Slime {
public:
    SlimeGreen(int x, int z, int id = 0);
};