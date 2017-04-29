#pragma once

#include "scene_camera.h"
#include <glm/glm.hpp>

struct SceneInfo {
    SceneCamera *camera;
    glm::vec3 light_pos;
};
