#pragma once

#include "projectile_trap.h"

/*
    Stationary trap that shoots close players in range
*/
class Shooter : public ProjectileTrap {
private:
    float attack_range;

    std::function<void()> cancel_turn;

    btBoxShape *hit_box = new btBoxShape(btVector3(0.2f, 0.2f, 0.2f));

    void check_sight(Player *player);
    void attack(Player *player);
    bool in_range(Player *player);
    void turn_to_shoot(Player *player);
public:
    Player *curr_target;

    Shooter(int x, int z, int id = 0);

    void setup_model() override;
    void play_trigger_animation() override;
    void update_ai() override;
};