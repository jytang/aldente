#include "gold.h"

namespace collectibles {

void Gold::collected_by(Player *player) {
    player->s_modify_stats([this](PlayerStats &stats) {
        stats.add_coins(amount);
    });
}

}