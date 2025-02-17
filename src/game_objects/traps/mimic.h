#pragma once

#include "mobile_trap.h"
#include "game_objects/player.h"

/* 
    A monster that will follow you around and attack if you open it.
    If you get away from it, then it will begin to target the closest player within
    a minimum range. (Currently, infinite range)
*/
class Mimic : public MobileTrap {
private:
    void s_to_attack_mode();
    void s_to_chest_mode();

    void c_to_chest();
    void c_to_mimic();
protected:

    Player *curr_target;

    void update_ai() override;

public:
    Mimic(int x, int z, int id = 0);

    void s_interact_trigger(GameObject *other) override;
    void c_interact_trigger(GameObject *other) override;

    void setup_model() override;

    void s_reset() override;
    void c_reset() override;
};