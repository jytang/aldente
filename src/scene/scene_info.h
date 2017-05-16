#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "camera.h"
#include "light/point_light.h"
#include "light/directional_light.h"
#include "light/spot_light.h"

class SceneInfo {
public:
    void update();
    Camera camera;
    std::vector<DirectionalLight *> dir_lights;
    std::vector<PointLight *> point_lights;
    std::vector<SpotLight *> spot_lights;
};