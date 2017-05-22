#pragma once

#include "collectible.h"

namespace collectibles {

class Gold : public Collectible {
public:
    Gold(int amount) : amount(amount) {}
    virtual void collected_by(Player *player) override;

private:
    int amount;
};

}
