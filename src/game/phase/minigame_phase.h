#pragma once

#include "phase.h"
#include "game/minigame/minigame.h"
#include "game/minigame/mg_penguin.h"
#include "game/context.h"

class MinigamePhase : public TimedPhase {
public:
    MinigamePhase(Context& context);
    ~MinigamePhase();

    void s_setup() override;
    proto::Phase s_update() override;
    void s_teardown() override;
    void c_setup() override;
    void c_update() override;
    void c_teardown() override;
private:
    std::vector<Minigame*> minigames;
    Minigame* curr_mg;
};