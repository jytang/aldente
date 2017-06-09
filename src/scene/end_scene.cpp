#include "end_scene.h"
#include "game_objects/player.h"
#include "events.h"
#include "util/color.h"
#include <input/modal_input.h>
#include "game_objects/minigame/platform.h"

EndScene::EndScene() : Scene() {
}

void EndScene::s_update() {
    Scene::s_update();
}

void EndScene::c_update() {
    Scene::c_update();
}

void EndScene::s_setup() {

}

void EndScene::c_setup() {
    placeholder = new Player(-1);

    Platform *platform = new Platform(PlatformShape::CIRCLE);
    platform->transform.set_scale(glm::vec3(0.0015f, 0.001f, 0.0015f));

    info.camera.cam_pos = glm::vec3(0.f, 0.f, 5.f);
    info.camera.cam_front = glm::vec3(0.f, 0.f, -1.f);
    info.camera.recalculate();

    // Setup lights.
    DirectionalLight *sun = new DirectionalLight(glm::vec3(0.f, -1.f, -1.f),
        Color::WHITE, 0.5f);
    add_light(sun);
}
