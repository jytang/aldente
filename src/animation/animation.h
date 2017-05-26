#pragma once

#include "glm/glm.hpp"
#include <assimp/scene.h>
#include <map>

/*
    Uses assimp's node structure to save animations to be played later.
    (Too much trouble to properly split the assimp animation node)
*/
class Animation {
public:
    Animation(const aiScene *scene, const aiAnimation *anim);

    const aiNode *get_root();
    const aiAnimation *get_anim();
    const aiNodeAnim *get_anim_node(std::string anim_name);

private:

    // For faster access to animation channels using stringsy
    std::map<std::string, const aiNodeAnim*> channels;

    const aiScene *scene;
    const aiAnimation *anim;
};