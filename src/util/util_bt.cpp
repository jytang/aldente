#include "util_bt.h"
#include <glm/gtc/type_ptr.hpp>

namespace util_bt {
    glm::vec3 convert_vec3(btVector3 vec) {
        return glm::vec3(vec.getX(), vec.getY(), vec.getZ());
    }

    glm::mat4 convert_trans(btTransform trans) {
        glm::mat4 transform;
        trans.getOpenGLMatrix(glm::value_ptr(transform));
    }
}