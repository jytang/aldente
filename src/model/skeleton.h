#pragma once

/*
    Contains bone information to be used for skeletal animations.
    Should always be attached to a game object that contains a
    compatible model.
*/

#include <GL/glew.h>
#include <map>
#include <vector>
#include "animation/animation.h"

struct Skeleton {
    std::map<std::string, Animation *> animations;

    // Skeleton Information
    glm::mat4 global_inv_trans; // Global Inverse Transform, Used for calculating bones_final
    std::map<std::string, GLuint> bone_mapping; // Maps bone name to index for easy access of bones
    std::vector<glm::mat4> bone_offsets; // Initial offset matrix for bone, used for calculating bones_final
    std::vector<glm::mat4> bones_default; // Defaults matrices for bones to be in default pose without animations
    std::vector<glm::mat4> bones_final; // Final transformation matrix for bone, including animation
};
