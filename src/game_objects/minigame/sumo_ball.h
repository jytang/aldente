#pragma once

/*
* SumoBall Class:
* These class represents the ball that players stand on
* in the "sumo" minigame.
*/

#include "../game_object.h"
#include "btBulletDynamicsCommon.h"
#include "events.h"
class SumoBall : public GameObject {
private: 
    Player *player;
    glm::vec3 prev_player_vec;
    bool first;
        
    std::vector<std::string> BALL_MODELS = { "lizard_ball", "pig_ball", "cat_ball", "tomato_ball" };
public:
    SumoBall(int id = 0);

    void c_update_this() override;

    void c_set_player(Player *to_set);
};