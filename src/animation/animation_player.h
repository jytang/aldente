#pragma once

#include "model/model.h"

/*
    Used for actually processing the animations and adjusting the bone matrices to actually
    pose the model
*/
class AnimationPlayer {
public:
    AnimationPlayer();

    void set_anim(Model *model, std::string anim_name);

    void update(); // Updates bone matrices for model based on animation playing
    void play(); // Tells the animation player to start playing
    void pause(); // Pauses the animation player without reseting animation loop
    void stop(); // Pauses the animation player and resets the animation loop

    void set_speed(float speed);
    void set_loop(bool speed);

    bool check_paused();

private:
    float last_time;
    float cur_time;
    
    Animation *animation; // Current Animation Being Played
    Model *model; // Current model playing animation

    float speed;
    bool loop;
    bool is_paused;
    
    void reset_model();

    void process_animation(float anim_time, const aiAnimation *anim, Model *model, const aiNode *node, glm::mat4 parent_mat);

    const aiNodeAnim *find_node_anim(const aiAnimation *anim, const std::string node_name);
    glm::mat4 convert_ai_matrix(aiMatrix4x4 ai_mat);

    // Math Calculation stuff taken from Assimp Tutorial
    // http://ogldev.atspace.co.uk/www/tutorial38/tutorial38.html
    void calc_interpolated_scaling(aiVector3D& out, float anim_time, const aiNodeAnim* node_anim);
    void calc_interpolated_rotation(aiQuaternion& out, float anim_time, const aiNodeAnim* node_anim);
    void calc_interpolated_position(aiVector3D& out, float anim_time, const aiNodeAnim* node_anim);
    unsigned int find_scaling(float anim_time, const aiNodeAnim* node_anim);
    unsigned int find_rotation(float anim_time, const aiNodeAnim* node_anim);
    unsigned int find_position(float anim_time, const aiNodeAnim* node_anim);
};