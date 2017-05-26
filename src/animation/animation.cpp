#include "animation.h"
#include <iostream>

Animation::Animation(const aiScene *scene, const aiAnimation *anim)
    : scene(scene), anim(anim) {

    // Saves node animations to map for easier access
    for (unsigned int i = 0; i < anim->mNumChannels; i++) {
        const aiNodeAnim* node_anim = anim->mChannels[i];
        channels[node_anim->mNodeName.data] = node_anim;
    }
}

const aiNode *Animation::get_root() {
    return scene->mRootNode;
}

const aiAnimation *Animation::get_anim() {
    return anim;
}

const aiNodeAnim *Animation::get_anim_node(std::string anim_name) {
    return channels[anim_name];
}