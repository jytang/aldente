#include "util_bt.h"
#include <glm/gtc/type_ptr.hpp>

namespace util_bt {
    glm::vec3 convert_vec3(btVector3 vec) {
        return glm::vec3(vec.getX(), vec.getY(), vec.getZ());
    }

    btVector3 convert_vec3(glm::vec3 vec) {
        return btVector3(vec.x, vec.y, vec.z);
    }

    // Not Tested
    glm::mat4 convert_trans(btTransform trans) {
        glm::mat4 transform;
        trans.getOpenGLMatrix(glm::value_ptr(transform));
        return transform;
    }

    // Not Tested
    btTransform convert_trans(glm::mat4 trans) {
        btTransform t;
        t.setFromOpenGLMatrix(glm::value_ptr(trans));
        return t;
    }
}