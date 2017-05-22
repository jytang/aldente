#pragma once

/*
* Player Class:
* This class will contain all functionality concerning a player 
* gameobject such as health, gold, movement, etc.
*/

#include "game_object.h"
#include "game/direction.h"
#include "btBulletDynamicsCommon.h"
#include "construct.h"
#include "events.h"
#include "game/player_stats.h"

class Player : public GameObject {
private:
    // to_move is for saving the joystick input in each frame.
    int to_moveX;
    int to_moveZ;

    float move_speed;

    glm::vec3 start_pos;

    btCapsuleShape *hit_capsule = new btCapsuleShape(0.5f, 1.0f);

    PlayerStats stats;
public:
    Player(int id = 0);

    void s_update_this() override;
    void c_update_state(float x, float z, float wx, float wz, bool enab) override;
    void s_on_collision(GameObject *other) override;
    void c_on_collision(GameObject *other) override;

    void setup_player_model(std::string &model_name); // Loads player model

    // Player movement
    void prepare_movement(int inX, int inZ);
    void do_movement();

    // Called when the player tries to interacts with an object (e.g pressing A)
    void interact();

    // Animation Cues
    void start_walk();
    void stop_walk();

    // TODO after we make more player models
    void setup_model() {}

    // Set this player's spawn position.
    void set_start_position(glm::vec3 pos);

    // Sets the player's position to its set start position.
    void reset_position();

    // Allows manipulation of stats through callback.
    // This is done so that the clients can receive any associated updates.
    // FOR SERVER USE ONLY.
    void s_modify_stats(std::function<void(PlayerStats &)> modifier);

    // Allows client to be updated with a server player status message.
    // FOR CLIENT USE ONLY.
    void c_update_stats(const proto::PlayerStats &update);
};
