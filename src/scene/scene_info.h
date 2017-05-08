#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "camera.h"
#include "point_light.h"
#include "directional_light.h"
#include "spot_light.h"

struct SceneInfo {
    Camera camera;
    std::vector<PointLight> point_lights;
    std::vector<DirectionalLight> dir_lights;
    std::vector<SpotLight> spot_lights;
};