#include "main_scene.h"
#include "game_objects/test_chest.h"
#include "game_objects/player.h"
#include "events.h"

MainScene::MainScene() : Scene() {
    
}

void MainScene::update() {

    Scene::update();

    //hover = physics.hover; hover = nullptr;
    std::vector<std::vector<Tile *>> toAdd = grid->getGrid();
    for (int i = 0; i < toAdd.size(); i++) {
        std::vector<Tile *> currRow = toAdd[i];
        for (int j = 0; j < currRow.size(); j++) {
            currRow[j]->update();
        }
    }

    grid->update();

    // Rotate light source just to test shadows.
    if (lights_debug_on)
        light_pos = glm::vec3(glm::rotate(glm::mat4(1.f), 0.01f, glm::vec3(0.f, 0.f, 1.f)) * glm::vec4(light_pos, 1.f));
}

void MainScene::setup_scene() {
    // Setup light debug callback.
    events::debug::toggle_light_rotation_event.connect([&](void) {
        lights_debug_on = !lights_debug_on;
    });

    light_pos = glm::vec3(0.f, 2.f, 1.f);

    //Setting up scene graph for Grid
    grid = new Grid(20, 20);
    std::vector<std::vector<Tile *>> toAdd = grid->getGrid();
    for (int i = 0; i < toAdd.size(); i++) {
        std::vector<Tile *> currRow = toAdd[i];
        for (int j = 0; j < currRow.size(); j++) {
            objs.push_back(currRow[j]);
        }
    }

    // Test Model loading
    Model *player_model = AssetLoader::get_model(std::string("boy_two_scaled"));
    Player *player = new Player();
    player_model->set_shader(&ShaderManager::anim_unlit);
    player->attach_model(player_model);
    //player->transform.set_scale({ 0.006f, 0.006f, 0.006f });
    player->start_walk();
    player->transform.translate({ 4.f, 0.05f, 4.f });
    objs.push_back(player);

    Model *chestg_model = AssetLoader::get_model(std::string("chest_good_scaled"));
    TestChest *chestg = new TestChest();
    chestg_model->set_shader(&ShaderManager::anim_unlit);
    chestg->attach_model(chestg_model);
    //chestg->transform.set_scale({ 0.006f, 0.006f, 0.006f });
    chestg->open_chest();
    chestg->transform.translate({ 5.f, 0.05f, 4.f });
    objs.push_back(chestg);

    Model *chestb_model = AssetLoader::get_model(std::string("chest_bad_scaled"));
    TestChest *chestb = new TestChest();
    chestb_model->set_shader(&ShaderManager::anim_unlit);
    chestb->attach_model(chestb_model);
    //ychestb->transform.set_scale({ 0.006f, 0.006f, 0.006f });
    chestb->open_chest();
    chestb->transform.translate({ 3.f, 0.05f, 4.f });
    objs.push_back(chestb);
}