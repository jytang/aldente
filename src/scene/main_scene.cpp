#include "main_scene.h"
#include "game_objects/test_chest.h"
#include "game_objects/player.h"
#include "game_objects/test_coin.h"
#include "events.h"
#include "util/color.h"
#include "light/pulse_point_light.h"

MainScene::MainScene() : Scene(),
    goal(nullptr) {

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

    // Attaching events
    events::dungeon::place_goal_event.connect([&]() {
        place_goal(glm::vec3(0.0f), 20);
    });

    events::dungeon::spawn_existing_goal_event.connect([&](int x, int z, int id) {
        std::unique_lock<std::mutex> lock(goal_mutex);
        place_existing_goal(x, z, id);
    });

    events::dungeon::remove_goal_event.connect([&](bool graphical) {
        std::unique_lock<std::mutex> lock(goal_mutex);
        if (graphical) {
            auto position = std::find(objs.begin(), objs.end(), goal);
            if (position != objs.end())
                objs.erase(position);
            delete goal;
            goal = nullptr;
        }
        else {
            remove_goal();
        }
    });
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
}

void MainScene::graphical_setup() {

    for (GameObject *obj : objs) {
        obj->setup_model();
    }
}

Player* MainScene::spawn_new_player() {
    Player *player = new Player();
    player->transform.set_scale({ 0.4f, 0.4f, 0.4f });
    player->transform.translate({ 2.f, 0.f, 2.f });
    objs.push_back(player);

    return player;
}

Player* MainScene::spawn_existing_player(int obj_id) {
    Player *player = new Player(obj_id);
    player->transform.set_scale({ 0.4f, 0.4f, 0.4f });
    player->transform.translate({ 2.f, 0.f, 2.f });
    objs.push_back(player);
    
    Model *player_model = AssetLoader::get_model(std::string("boy_two"));
    player_model->set_shader(&ShaderManager::anim_unlit);
    player->attach_model(player_model);
    player->start_walk();
    
    return player;
}

void MainScene::place_goal(glm::vec3 start, int min_dist) {
    // Goal will be in range of (min_dist, edge of map)
    int new_goal_x = rand() % grid->get_width();
    int new_goal_z = rand() % grid->get_height();

    std::vector<std::vector<Tile*>> grid_array = grid->get_grid();
    // If not buildable or too close, find another
    while (!grid_array[new_goal_z][new_goal_x]->isBuildable() ||
        (abs(new_goal_x - start.x) + abs(new_goal_z - start.z) < min_dist)) {
        new_goal_x = rand() % grid->get_width();
        new_goal_z = rand() % grid->get_height();
    }

    Goal *new_goal = new Goal(new_goal_x, new_goal_z);

    (grid->get_grid())[new_goal_z][new_goal_x]->set_construct(new_goal);
    goal = new_goal;
    goal_x = new_goal_x;
    goal_z = new_goal_z;
}

void MainScene::place_existing_goal(int x, int z, int id) {
    Goal *new_goal = new Goal(x, z, id);
    goal = new_goal;

    new_goal->setup_model();
    objs.push_back(new_goal);
}

void MainScene::remove_goal() {
    //TODO destructor for goal
    if (goal) {
        (grid->get_grid())[goal_z][goal_x]->set_construct(nullptr);
        events::remove_rigidbody_event(goal);
    }
}
