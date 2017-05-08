#include "scene_info.h"

void SceneInfo::update() {
    camera.update();
    for (int i = 0; i < dir_lights.size(); ++i)
        dir_lights[i].update();
    for (int i = 0; i < point_lights.size(); ++i)
        point_lights[i].update();
    for (int i = 0; i < spot_lights.size(); ++i)
        spot_lights[i].update();
}