#pragma once

#include "game_objects/game_object.h"

class TestCoin : public GameObject {
public:
    TestCoin();

    void spin_coin();
private:
    virtual void update_this() override;
    virtual void setup_model() override {};
};