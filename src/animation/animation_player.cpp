#include "animation_player.h"
#include <GLFW/glfw3.h>
#include <iostream>

AnimationPlayer::AnimationPlayer() {
    last_time = 0.0f;
    cur_time = 0.0f;

    // Default Values
    speed = 1.0f;
    loop = false;
    is_paused = true;
}

// Currently, animation player keeps track of its own timer. May change to a global timer event.
void AnimationPlayer::update() {

    // No animation detected
    if (!animation)
        return;

    if (!is_paused) {
        cur_time += ((float)glfwGetTime() - last_time) * speed;

        float ticks_per_sec = (float)(animation->get_anim()->mTicksPerSecond);
        float time_in_ticks = cur_time *  ticks_per_sec;

        // Check if Animation has Ended
        if (time_in_ticks > animation->get_anim()->mDuration) {
            if (!loop) {
                stop();
                return;
            }
        }

        float animation_time = fmod(time_in_ticks, (float)animation->get_anim()->mDuration);

        process_animation(animation_time, animation, animation->get_root(), glm::mat4(1.0f));
    }

    last_time = glfwGetTime();
}

void AnimationPlayer::set_skeleton(Skeleton *skel) {
    this->skel = skel;
}

void AnimationPlayer::set_anim(std::string anim_name) {
    if (!skel) return; // cannot set animation without a skeleton

    animation = skel->animations[anim_name];

    if (animation == NULL) {
        std::cerr << "Error : Animation " << anim_name << " not found." << std::endl;
        return;
    }

    stop();
}

void AnimationPlayer::play() {
    is_paused = false;
    last_time = glfwGetTime();
}

void AnimationPlayer::play_if_paused(std::string anim_name, float speed) {
    if (check_paused()) {
        set_anim(anim_name);
        set_speed(speed);
        play();
    }
}

void AnimationPlayer::pause() {
    is_paused = true;
}

void AnimationPlayer::stop() {
    is_paused = true;
    cur_time = 0.0f;    
    reset_model();
}

void AnimationPlayer::set_speed(float speed) {
    this->speed = speed;
}

void AnimationPlayer::set_loop(bool will_loop) {
    this->loop = will_loop;
}

bool AnimationPlayer::check_paused() {
    return is_paused;
}

// Resets the model to its default pose, without animations
void AnimationPlayer::reset_model() {
    if (!skel)
        return;

    for (int i = 0; i < skel->bones_final.size(); i++) {
        skel->bones_final[i] = glm::mat4(1.0f);
    }    
}

// Processes Animation Node using Assimp's node structure
void AnimationPlayer::process_animation(float anim_time, Animation *anim, const aiNode *node, glm::mat4 parent_mat) {

    if (!anim)
        return;

    std::string node_name(node->mName.data);

    glm::mat4 node_trans = convert_ai_matrix(node->mTransformation);

    const aiNodeAnim *node_anim = anim->get_anim_node(node_name);

    if (node_anim) {
        // Interpolate scaling and generate scaling transformation matrix
        aiVector3D scaling;
        calc_interpolated_scaling(scaling, anim_time, node_anim);
        glm::mat4 scaling_mat = glm::scale(glm::mat4(1.0f), glm::vec3(scaling.x, scaling.y, scaling.z));

        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion rotation;
        calc_interpolated_rotation(rotation, anim_time, node_anim);
        glm::mat4 rotation_mat = glm::mat4(convert_ai_matrix(aiMatrix4x4(rotation.GetMatrix())));

        // Interpolate translation and generate translation transformation matrix
        aiVector3D translation;
        calc_interpolated_position(translation, anim_time, node_anim);
        glm::mat4 trans_mat = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z));

        // Combine the above transformations
        node_trans = trans_mat * rotation_mat * scaling_mat;
    }

    glm::mat4 global_trans = parent_mat * node_trans;

    if (skel->bone_mapping.find(node_name) != skel->bone_mapping.end()) {
        unsigned int bone_index = skel->bone_mapping[node_name];

        // Calculates final bone transformation matrix. Requires inverse of bones_default
        // because shader multiples by mesh_model by default, and that is actually already 
        // incorporated in global_trans, therefore needs to be counteracted with the inverse.
        skel->bones_final[bone_index] =  glm::inverse(skel->bones_default[bone_index]) *
            skel->global_inv_trans * global_trans * skel->bone_offsets[bone_index];
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        process_animation(anim_time, anim, node->mChildren[i], global_trans);
    }
}

unsigned int AnimationPlayer::find_position(float anim_time, const aiNodeAnim *node_anim) {
    for (unsigned int i = 0; i < node_anim->mNumPositionKeys - 1; i++) {
        if (anim_time < (float)node_anim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }
    
    // If animation node is structured correctly, this should never be called
    std::cerr << "Error : Position Key for " << node_anim->mNodeName.data << " not found." << std::endl;
    return 0;
}

unsigned int AnimationPlayer::find_rotation(float anim_time, const aiNodeAnim *node_anim) {
    for (unsigned int i = 0; i < node_anim->mNumRotationKeys - 1; i++) {
        if (anim_time < (float)node_anim->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }

    // If animation node is structured correctly, this should never be called
    std::cerr << "Error : Rotation Key for " << node_anim->mNodeName.data << " not found." << std::endl;
    return 0;
}

unsigned int AnimationPlayer::find_scaling(float anim_time, const aiNodeAnim *node_anim) {
    for (unsigned int i = 0; i < node_anim->mNumScalingKeys - 1; i++) {
        if (anim_time < (float)node_anim->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }

    // If animation node is structured correctly, this should never be called
    std::cerr << "Error : Scaling Key for " << node_anim->mNodeName.data << " not found." << std::endl;
    return 0;
}


void AnimationPlayer::calc_interpolated_position(aiVector3D &out, float anim_time, const aiNodeAnim *node_anim) {
    if (node_anim->mNumPositionKeys == 1) {
        out = node_anim->mPositionKeys[0].mValue;
        return;
    }

    unsigned int position_index = find_position(anim_time, node_anim);
    unsigned int next_position_index = (position_index + 1);
    if (next_position_index >= node_anim->mNumPositionKeys) {
        // If animation node is structured correctly, this should never be called
        std::cerr << "Error : Position Index for " << node_anim->mNodeName.data << " incorrect." << std::endl;
    }

    float deltaTime = (float)(node_anim->mPositionKeys[next_position_index].mTime - node_anim->mPositionKeys[position_index].mTime);
    float factor = (anim_time - (float)node_anim->mPositionKeys[position_index].mTime) / deltaTime;

    if (factor < 0.0f || factor > 1.0f) {
        // If animation node is structured correctly, this should never be called
        std::cerr << "Error : Position Factor for " << node_anim->mNodeName.data << " incorrect." << std::endl;
    }

    const aiVector3D &start = node_anim->mPositionKeys[position_index].mValue;
    const aiVector3D &end = node_anim->mPositionKeys[next_position_index].mValue;
    aiVector3D delta = end - start;
    out = start + factor * delta;
}


void AnimationPlayer::calc_interpolated_rotation(aiQuaternion &out, float anim_time, const aiNodeAnim *node_anim) {
    // we need at least two values to interpolate...
    if (node_anim->mNumRotationKeys == 1) {
        out = node_anim->mRotationKeys[0].mValue;
        return;
    }

    unsigned int rotation_index = find_rotation(anim_time, node_anim);
    unsigned int next_rotation_index = (rotation_index + 1);
    
    if (next_rotation_index >= node_anim->mNumRotationKeys) {
        // If animation node is structured correctly, this should never be called
        std::cerr << "Error : Rotation Index for " << node_anim->mNodeName.data << " incorrect." << std::endl;
    }

    float deltaTime = (float)(node_anim->mRotationKeys[next_rotation_index].mTime - node_anim->mRotationKeys[rotation_index].mTime);
    float factor = (anim_time - (float)node_anim->mRotationKeys[rotation_index].mTime) / deltaTime;

    if (factor < 0.0f || factor > 1.0f) {
        // If animation node is structured correctly, this should never be called
        std::cerr << "Error : Rotation Factor for " << node_anim->mNodeName.data << " incorrect." << std::endl;
    }

    const aiQuaternion &startRotationQ = node_anim->mRotationKeys[rotation_index].mValue;
    const aiQuaternion &endRotationQ = node_anim->mRotationKeys[next_rotation_index].mValue;
    aiQuaternion::Interpolate(out, startRotationQ, endRotationQ, factor);
    out = out.Normalize();
}


void AnimationPlayer::calc_interpolated_scaling(aiVector3D &out, float anim_time, const aiNodeAnim *node_anim) {
    if (node_anim->mNumScalingKeys == 1) {
        out = node_anim->mScalingKeys[0].mValue;
        return;
    }

    unsigned int scaling_index = find_scaling(anim_time, node_anim);
    unsigned int next_scaling_index = (scaling_index + 1);

    if (next_scaling_index >= node_anim->mNumScalingKeys) {
        // If animation node is structured correctly, this should never be called
        std::cerr << "Error : Scaling Index for " << node_anim->mNodeName.data << " incorrect." << std::endl;
    }

    float deltaTime = (float)(node_anim->mScalingKeys[next_scaling_index].mTime - node_anim->mScalingKeys[scaling_index].mTime);
    float factor = (anim_time - (float)node_anim->mScalingKeys[scaling_index].mTime) / deltaTime;

    if (factor < 0.0f || factor > 1.0f) {
        // If animation node is structured correctly, this should never be called
        std::cerr << "Error : Scaling Factor for " << node_anim->mNodeName.data << " incorrect." << std::endl;
    }

    const aiVector3D &start = node_anim->mScalingKeys[scaling_index].mValue;
    const aiVector3D &end = node_anim->mScalingKeys[next_scaling_index].mValue;
    aiVector3D delta = end - start;
    out = start + factor * delta;
}

glm::mat4 AnimationPlayer::convert_ai_matrix(aiMatrix4x4 ai_mat) {
    return{ ai_mat.a1, ai_mat.b1, ai_mat.c1, ai_mat.d1,
        ai_mat.a2, ai_mat.b2, ai_mat.c2, ai_mat.d2,
        ai_mat.a3, ai_mat.b3, ai_mat.c3, ai_mat.d3,
        ai_mat.a4, ai_mat.b4, ai_mat.c4, ai_mat.d4 };
}
