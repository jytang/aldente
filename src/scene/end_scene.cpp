#include "end_scene.h"
#include "game_objects/player.h"
#include "events.h"
#include "util/color.h"
#include <input/modal_input.h>
#include "game_objects/minigame/platform.h"

EndScene::EndScene() : Scene() {
    cancel_timer = []() {};
    angle = 0;
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
    placeholder->transform.set_position(glm::vec3(0,1.5f,0));
    objs.push_back(placeholder);

    Platform *platform = new Platform(PlatformShape::CIRCLE);
    platform->transform.set_scale(glm::vec3(0.005f, 0.01f, 0.005f));
    platform->transform.set_position(glm::vec3(0));
    platform->setup_model();
    objs.push_back(platform);

    // Setup lights.
    DirectionalLight *sun = new DirectionalLight(glm::vec3(0.f, -1.f, -1.f),
        Color::WHITE, 0.5f);
    add_light(sun);

    int count = 0;
    cancel_timer = Timer::get()->do_every(
            std::chrono::milliseconds(100),
            [&, count]() mutable {
        placeholder->c_setup_player_model(count++);
        if (count > 3) {
            count = 0;
        }
    });
}

void EndScene::reset_camera() {
    info.camera.reset();
    info.camera.cam_pos = glm::vec3(0.f, 2.f, 5.f);

    info.camera.recalculate();
}

void EndScene::cancel_cycle() {
    cancel_timer();
    Timer::get()->do_every(
        std::chrono::milliseconds(30),
        [&]() {
        for (GameObject *obj : objs) {
            obj->transform.rotate(glm::vec3(0, 1, 0), 5.0f);
        }
    });
};