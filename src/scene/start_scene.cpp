#include "start_scene.h"
#include "game_objects/player.h"
#include "events.h"
#include "util/color.h"

StartScene::StartScene() : Scene(), menu_enabled(true) {
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
        platform->transform.set_position(glm::vec3(2 * i, -2.2f, 0));
        platform->attach_model(AssetLoader::get_model("bed"));
        platform->transform.set_scale({ 0.015f, 0.015f, 0.015f });

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
        menu_enabled = false;
        events::camera_anim_rotate_event(glm::vec3(0, 1, 0), 180, 1500, [&](){
            // If we're connected already, focus camera to player.
            if (client_player)
                zoom_to_client_player(client_player);
            else
                events::camera_anim_position_event(glm::vec3(3.f, 0.f, 6.f), 1500, [](){});
        });
    });

    // Zoom camera out to see all beds if player readied up.
    events::player_ready_event.connect([&](int player_id) {
        Player* player = dynamic_cast<Player*>(GameObject::game_objects[player_id]);
        if (client_player && client_player == player) {
            events::camera_anim_position_event(glm::vec3(3.f, 0.f, 6.f), 1500, []() {});
        }
    });
}

void StartScene::zoom_to_client_player(Player* player) {
    if (menu_enabled) {
        client_player = player;
        return;
    }

    auto player_pos = client_player->transform.get_position();
    player_pos.z += 4;
    events::camera_anim_position_event(player_pos, 1500, []() {});
}
