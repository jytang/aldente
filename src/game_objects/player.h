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

    std::function<void()> cancel_flicker; // Callback function to cancel damage flicker.
    std::function<void()> cancel_stun;
    std::function<void()> cancel_invulnerable;
    std::function<void()> cancel_slow;
    std::function<void()> cancel_confuse;


    bool end_flicker; // Whether we should stop player flickering.
    bool invulnerable; // Can't lose coins or collide with other harmful traps.
    bool stunned; // Can't move.
    bool anim_override; // Always animate
    bool confused; // Movement direction is reversed

    // to_move is for saving the joystick input in each frame.
    int to_moveX;
    int to_moveZ;

    float move_speed;

    bool exiting;
    bool is_client;

    btCapsuleShape *hit_capsule = new btCapsuleShape(0.2f, 1.0f);
    btCapsuleShape *sumo_hit_capsule = new btCapsuleShape(0.45f, 3.0f);

    PlayerStats stats;
    int model_index;

    bool momentum;
    bool sumo;

    void set_confuse_effect(bool b);
    
    // sets whether ice effect shows for player
    void c_set_ice_effect(bool b);

public:
    Player(int id = 0);

    void s_update_this() override;
    void c_update_state(glm::mat4 mat, bool enab) override;
    void s_on_collision(GameObject *other) override;
    void c_on_collision(GameObject *other) override;

    void s_reset() override;
    void c_reset() override;

    void c_setup_player_model(int index); // Loads player model

    bool get_exiting_status() { return exiting; };
    void set_exiting_status(bool to_set) { exiting = to_set; };

    // Player movement
    void prepare_movement(int inX, int inZ);
    void do_movement();

    // Called when the player tries to interacts with an object (e.g pressing A)
    void interact();

    // Animation Cues
    void start_walk();
    void stop_walk();
    void set_pump();

    // Set this player's spawn position.
    void set_start_position(glm::vec3 pos);

    // Sets the player's position to its set start position.
    void reset_position();

    // Sets up the warping animation and signals phase
    void s_begin_warp(float x, float z);
    void c_begin_warp();

    // Damage functions
    bool s_take_damage(); // Set invulnerability, drop coins, lose gold (true if took damage)
    void c_take_damage(); // Graphical : Flickers player to show they've been hit

    // Get Slowed
    void s_slow(); // Hit by slow trap. Get frozen, and then slowly regain movespeed
    void c_slow(); // Turn bluish to indicate frozen. Can later change to a UI effect
    
    // Get Confused
    bool s_confuse(); // Movement gets reversed.
    void c_confuse(); // Turns purple, with purple UI overlay

    // Used for checking if player has been hit and in the period of invulnerability, 
    // where they can't be hit by traps again.
    bool is_invulnerable() { return invulnerable; };

    // Used for checking if player has been stunned
    // since they cannot pick up essence during this time or interact w objects.
    bool is_stunned() { return stunned; };

    // Allows manipulation of stats through callback.
    // This is done so that the clients can receive any associated updates.
    // FOR SERVER USE ONLY.
    void s_modify_stats(std::function<void(PlayerStats &)> modifier);

    // Allows client to be updated with a server player status message.
    // FOR CLIENT USE ONLY.
    void c_update_stats(const proto::PlayerStats &update);

    // Checks if a player has enough funds to afford a cost.
    bool can_afford(int cost);

    // Get player's current gold.
    int get_coins();

    // Client side. Whether or not this player is the client player.
    void c_set_client_player();

    // Set the player's model index.
    void s_set_model_index(int index);
    int c_get_model_index() const;

    static std::vector<std::string> PLAYER_MODELS;

    void set_speed(float to_set) { move_speed = to_set; };
    void set_momentum(bool to_set) { momentum = to_set; };

    void toggle_sumo_collider();
    void set_anim_override(bool to_set) { anim_override = to_set; };
};
