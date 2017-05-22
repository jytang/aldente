#pragma once

class PlayerStats {
public:
    PlayerStats()
            : coins(0) {}

    int get_coins();
    int add_coins(int delta);
    int set_coins(int amount);
private:
    int coins;
};
