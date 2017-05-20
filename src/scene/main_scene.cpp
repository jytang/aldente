#include "main_scene.h"
#include "game_objects/test_chest.h"
#include "game_objects/player.h"
#include "game_objects/test_coin.h"
#include "events.h"
#include "util/color.h"
#include "light/pulse_point_light.h"

MainScene::MainScene() : Scene() {

}

void MainScene::update() {
    Scene::update();
}

void MainScene::client_update() {
    Scene::client_update();
    grid->update();

    // Rotate directional light sources just to test shadows.
    if (lights_debug_on) {
        for (int i = 0; i < info.dir_lights.size(); ++i) {
            info.dir_lights[i]->transform.rotate(0.f, 0.f, 0.01f, false);
        }
    }
}

void MainScene::setup_scene() {
    //Setting up map
    grid = new Grid("assets/maps/dungeon_test.txt");
    objs.push_back(grid);

}

void MainScene::graphical_setup() {
    // Setup lights.
    DirectionalLight *sun = new DirectionalLight(glm::vec3(0.f, -1.f, -1.f),
                                                 Color::WHITE, 0.5f);
    add_light(sun);

    PointLight *bulb = new PointLight(glm::vec3(5.f, 2.f, 5.f), Color::BONE_WHITE, 0.1f);
    add_light(bulb);

    PulsePointLight *bulb2 = new PulsePointLight(glm::vec3(7.f, 2.f, 10.f), Color::WINDWAKER_GREEN);
    add_light(bulb2);

    SpotLight *spot_light = new SpotLight(glm::vec3(10.f, 4.f, 5.f),
                                          glm::vec3(0.f, -1.f, 0.f),
                                          Color::MAGENTA);
    add_light(spot_light);

    // Setup light debug callback.
    events::debug::toggle_light_rotation_event.connect([&](void) {
        lights_debug_on = !lights_debug_on;
    });

    for (GameObject *obj : objs) {
        obj->setup_model();
    }
}

Player* MainScene::spawn_new_player(int conn_id) {
    Player *player = new Player();
    player->transform.set_scale({ 0.4f, 0.4f, 0.4f });

    // TODO: determine where each player starts based on client id. 
    // For now, players 1-4 start at (2, 2), (2, 3), (2, 4), (2, 5) respectively.
    player->set_start_position({ 2.f, 0, 1.f + conn_id });
    player->reset_position();
    objs.push_back(player);

    return player;
}

Player* MainScene::spawn_existing_player(int obj_id) {
    Player *player = new Player(obj_id);
    player->transform.set_scale({ 0.004f, 0.004f, 0.004f });
    player->transform.translate({ 2.f, 0.f, 2.f });
    objs.push_back(player);
    
    Model *player_model = AssetLoader::get_model(std::string("cat"));
    player_model->set_shader(&ShaderManager::anim_unlit);
    player->attach_model(player_model);
    player->start_walk();
    
    return player;
}
