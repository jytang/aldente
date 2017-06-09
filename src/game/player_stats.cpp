#include "player_stats.h"

#define MAX_COINS 10000

int PlayerStats::get_coins() {
    return coins;
}

void PlayerStats::add_coins(int delta) {
    coins += delta;
    clamp();
}

void PlayerStats::set_coins(int amount) {
    coins = amount;
    clamp();
}

void PlayerStats::clamp() {
    if (coins < 0)
        coins = 0;
    if (coins > MAX_COINS)
        coins = MAX_COINS;
}
