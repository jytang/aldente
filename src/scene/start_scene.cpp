#include "start_scene.h"
#include "game_objects/player.h"
#include "events.h"
#include "util/color.h"

StartScene::StartScene() : Scene() {
}

void StartScene::s_update() {
    Scene::s_update();
}

void StartScene::c_update() {
    Scene::c_update();
}

void StartScene::s_setup() {    
}

void StartScene::c_setup() {
    // Set up platforms on which the player will stand.
    // Serves no functionality other than just being visible.
    for (int i = 0; i < 4; i++) {
        GameObject *platform = new GameObject(-1);
        platform->transform.set_position(glm::vec3(2 * i, -0.8f, 0));
        platform->attach_model(AssetLoader::get_model("chest_good"));
        platform->transform.set_scale({ 0.006f, 0.006f, 0.006f });

        objs.push_back(platform);
        platforms.push_back(platform);
    }

    for (GameObject *obj : platforms) {
        obj->setup_model();
    }

    info.camera.cam_pos = glm::vec3(3.f, 0, 50.f);
    info.camera.cam_front = glm::vec3(0.f, 0.f, 1.f);
    info.camera.recalculate();

    // Setup lights.
    DirectionalLight *sun = new DirectionalLight(glm::vec3(0.f, -1.f, -1.f),
        Color::WHITE, 0.5f);
    add_light(sun);

    // Main menu ui should show.
    events::ui::enable_main_menu();

    // Transition camera when main menu is done.
    events::ui::disable_main_menu.connect([&]() {
        events::camera_anim_rotate_event(glm::vec3(0, 1, 0), 180, 1500, [](){
            events::camera_anim_position_event(glm::vec3(3.f, 0.f, 6.f), 1500, [](){});
        });
    });
}
