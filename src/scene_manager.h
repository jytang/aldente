#pragma once

#include "scene/scene.h"
#include "scene/main_scene.h"
#include "scene/camera.h"

class SceneManager {
private:
    std::vector<Scene *> scenes;
    Scene *current_scene;
    Camera *camera;

public:
    SceneManager();
    std::vector<Scene *> get_scenes(); // Gets list of all scenes in the game
    Scene *get_current_scene(); // Gets currently active scene
    Camera *get_camera(); // Gets currently active camera

    void add_scene(Scene* scene);
    void set_current_scene(Scene* scene);
};
