#include "scene.h"

void Scene::draw(Shader *shader) {
    for (GameObject *gameObj : objs) {
        gameObj->draw(shader, info);
    }
}

void Scene::draw_skybox() {
    ShaderManager::skybox.use();
    ShaderManager::skybox.draw(NULL, info); // NULL because no mesh needed.
}

Camera &Scene::get_cam() {
    return info.camera;
}

void Scene::update() {
    for (GameObject *gameObj : objs) {
        gameObj->update();
    }
}

void Scene::add_light(DirectionalLight l) {
    if (info.dir_lights.size() < MAX_DIR_LIGHTS)
        info.dir_lights.push_back(l);
    else
        assert(false); // stop adding so many lights, you lightmongerer!
}

void Scene::add_light(PointLight l) {
    if (info.point_lights.size() < MAX_POINT_LIGHTS)
        info.point_lights.push_back(l);
    else
        assert(false); // stop adding so many lights, light-mongrel!
}

void Scene::add_light(SpotLight l) {
    if (info.spot_lights.size() < MAX_SPOT_LIGHTS)
        info.spot_lights.push_back(l);
    else
        assert(false); // stop adding so many lights, light-advocate!
}