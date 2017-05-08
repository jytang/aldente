#include "scene_manager.h"

#include "events.h"
#include "util/config.h"

SceneManager::SceneManager() {
    // Setup callbacks
    events::window_buffer_resize_event.connect([&](events::WindowSizeData d) {
        float far_plane, fov;
        Config::config->get_value(Config::str_far_plane, far_plane);
        Config::config->get_value(Config::str_fov, fov);

        if (d.height > 0) {
            for (Scene *s : get_scenes())
                s->get_cam().P = glm::perspective(fov,
                        (float) d.width / (float) d.height,
                        0.1f, far_plane);
        }
    });
}

std::vector<Scene *> SceneManager::get_scenes() {
    return scenes;
}

Scene *SceneManager::get_current_scene() {
    return current_scene;
}

Camera *SceneManager::get_camera() {
    return camera;
}

void SceneManager::add_scene(Scene* scene) {
    //Make sure you're not adding a duplicate scene
    assert(std::find(scenes.begin(), scenes.end(), scene) == scenes.end());
    scenes.push_back(scene);
}

void SceneManager::set_current_scene(Scene* scene) {
    //Add the scene if not currently added
    if (std::find(scenes.begin(), scenes.end(), scene) == scenes.end()) {
        add_scene(scene);
    }
    current_scene = scene;
    camera = &scene->get_cam();
    scene->setup_scene();
}
