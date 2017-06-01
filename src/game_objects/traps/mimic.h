#pragma once

#include "mobile_trap.h"
#include "game_objects/player.h"

/* 
    A monster that will follow you around and attack if you open it.
    If you get away from it, then it will begin to target the closest player within
    a minimum range. 
*/
class Mimic : public MobileTrap {
protected:

    Player *curr_target;

    void update_ai() override;
    void change_direction() override;

public:
    Mimic(int x, int z, int id = 0);

    void c_update_state(glm::mat4 mat, bool enab);

    void s_interact_trigger(GameObject *other) override;

    void setup_model() override;

    void c_reset() override;
};