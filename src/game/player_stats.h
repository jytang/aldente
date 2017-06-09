#pragma once

class PlayerStats {
public:
    PlayerStats()
            : coins(0) {}

    int get_coins();
    void add_coins(int delta);
    void set_coins(int amount);
private:
    int coins;
};
