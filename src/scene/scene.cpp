#include "scene.h"

Scene::~Scene() {
    for (DirectionalLight *l : info.dir_lights)
        delete l;
    for (PointLight *l : info.point_lights)
        delete l;
    for (SpotLight *l : info.spot_lights)
        delete l;
}

void Scene::draw(Shader *shader) {
    for (GameObject *gameObj : objs) {
        gameObj->c_draw(shader, info);
    }
}

void Scene::draw_skybox() {
    ShaderManager::skybox.use();
    ShaderManager::skybox.draw(NULL, info); // NULL because no mesh needed.
}

Camera &Scene::get_cam() {
    return info.camera;
}

void Scene::s_update() {
    for (GameObject *gameObj : objs) {
        gameObj->s_update();
    }
}

void Scene::c_update() {
    info.update();
    for (GameObject *gameObj : objs) {
        gameObj->c_update();
    }
}

void Scene::add_light(DirectionalLight *l) {
    if (info.dir_lights.size() < MAX_DIR_LIGHTS)
        info.dir_lights.push_back(l);
    else
        assert(false); // stop adding so many lights, you lightmongerer!
}

void Scene::add_light(PointLight *l) {
    if (info.point_lights.size() < MAX_POINT_LIGHTS)
        info.point_lights.push_back(l);
    else
        assert(false); // stop adding so many lights, light-mongrel!
}

void Scene::add_light(SpotLight *l) {
    if (info.spot_lights.size() < MAX_SPOT_LIGHTS)
        info.spot_lights.push_back(l);
    else
        assert(false); // stop adding so many lights, light-advocate!
}

void Scene::remove_light(DirectionalLight *l) {
    auto position = std::find(info.dir_lights.begin(), info.dir_lights.end(), l);
    if (position != info.dir_lights.end())
        info.dir_lights.erase(position);
}

void Scene::remove_light(PointLight *l) {
    auto position = std::find(info.point_lights.begin(), info.point_lights.end(), l);
    if (position != info.point_lights.end())
        info.point_lights.erase(position);
}

void Scene::remove_light(SpotLight *l) {
    auto position = std::find(info.spot_lights.begin(), info.spot_lights.end(), l);
    if (position != info.spot_lights.end())
        info.spot_lights.erase(position);
}

void Scene::disable_scene() {
    for (GameObject *obj : objs) {
        obj->disable_all();
    }
}

void Scene::enable_scene() {
    for (GameObject *obj : objs) {
        obj->enable_all();
    }
}