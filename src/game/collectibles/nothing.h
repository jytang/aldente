#pragma once

#include "collectible.h"

namespace collectibles {

class Nothing : public Collectible {
public:
    void collected_by(Player *player) override {}
};

}
