#pragma once

/*
* Basket class: Basket part of the hotair balloon minigame
*/

#include "../game_object.h"

class HotAirBasket : public GameObject {
public:
    HotAirBasket(int id);

    void setup_model() override;

    void reset();
};