#include "main_scene.h"
#include "game_objects/player.h"
#include "events.h"
#include "util/color.h"

std::vector<std::string> MainScene::player_models = { "tomato", "cat", "boy_two" };

const std::string map_test = "assets/maps/dungeon_test.txt"; // 20x30 First map we had
const std::string map_wtf = "assets/maps/dungeon_wtf.txt"; // 60x60 Very big one with random stuff
const std::string map_1 = "assets/maps/dungeon_1.txt"; // 40x40, Starting in center with various ideas
const std::string map_2 = "assets/maps/dungeon_2.txt"; // 40x40, Most normal map, big spaces and narrow spaces
const std::string map_3 = "assets/maps/dungeon_3.txt"; // 40x40, Chaos Map
const std::string map_4 = "assets/maps/dungeon_4.txt"; // 30x30, Smaller rendition of a part of Map_2

std::string chosen_map = map_4; // Choose Map here

MainScene::MainScene() : Scene(), goal(nullptr) {
    events::dungeon::s_prepare_dungeon_event.connect([&]() {
        remove_goal();
        s_place_goal(glm::vec3(0.0f), 20);
    });

    events::dungeon::spawn_existing_goal_event.connect([&](int x, int z, int id) {
        std::unique_lock<std::mutex> lock(goal_mutex);
        if (goal) {
            auto position = std::find(objs.begin(), objs.end(), goal);
            if (position != objs.end())
                objs.erase(position);
            goal = nullptr;
        }
        if (goal_light)
            remove_light(goal_light);

        c_place_goal(x, z, id);
    });
}

void MainScene::s_update() {
    Scene::s_update();
}

void MainScene::c_update() {
    Scene::c_update();

    // Rotate directional light sources just to test shadows.
    if (lights_debug_on) {
        for (int i = 0; i < info.dir_lights.size(); ++i) {
            info.dir_lights[i]->transform.rotate(0.f, 0.f, 0.01f, false);
        }
    }
}

void MainScene::s_setup() {
    //Setting up map
    grid = new Grid(chosen_map.c_str());
    objs.push_back(grid);
}

void MainScene::c_setup() {
    //Setting up map
    grid = new Grid(chosen_map.c_str());
    objs.push_back(grid);

    // Setup lights.
    DirectionalLight *sun = new DirectionalLight(glm::vec3(0.f, -1.f, -1.f),
                                                 Color::WHITE, 0.5f);
    add_light(sun);

    // Setup light debug callback.
    events::debug::toggle_light_rotation_event.connect([&](void) {
        lights_debug_on = !lights_debug_on;
    });

    for (GameObject *obj : objs) {
        obj->setup_model();
    }
}

Player* MainScene::s_spawn_player(int conn_id) {
    Player *player = new Player();

    // TODO: determine where each player starts based on client id. 
    // For now, players 1-4 start at (2, 2), (2, 3), (2, 4), (2, 5) respectively.
    player->set_start_position({ 2.f, 0, 1.f + conn_id });
    player->s_set_model_name(player_models[conn_id % 3]);   // TODO: temporary 3 because no lizard
    player->reset_position();
    objs.push_back(player);

    return player;
}

Player* MainScene::c_spawn_player(int obj_id, std::string& model_name) {
    Player *player = new Player(obj_id);
    player->setup_player_model(model_name);
    objs.push_back(player);

    return player;
}

void MainScene::s_place_goal(glm::vec3 start, int min_dist) {
    // Goal will be in range of (min_dist, edge of map)
    int new_goal_x = rand() % grid->get_width();
    int new_goal_z = rand() % grid->get_height();

    // If not buildable or too close, find another
    while (!grid->get_grid()[new_goal_z][new_goal_x]->isBuildable() ||
        (abs(new_goal_x - start.x) + abs(new_goal_z - start.z) < min_dist)) {
        new_goal_x = rand() % grid->get_width();
        new_goal_z = rand() % grid->get_height();
    }

    Goal *new_goal = new Goal(new_goal_x, new_goal_z);

    grid->get_grid()[new_goal_z][new_goal_x]->set_construct(new_goal);
    goal = new_goal;
    goal_x = new_goal_x;
    goal_z = new_goal_z;
}

void MainScene::c_place_goal(int x, int z, int id) {
    goal = new Goal(x, z, id);

    goal_light = 
        new PulsePointLight(glm::vec3(x, 0.5f, z), Color::OCEAN_BLUE, 0, 0.15f, 0.0055f, 0.1f);
    add_light(goal_light);

    goal->setup_model();
    objs.push_back(goal);
}

void MainScene::remove_goal() {
    //TODO destructor for goal
    if (goal) {
        GameObject::game_objects.erase(goal->get_id());
        grid->get_grid()[goal_z][goal_x]->set_construct(nullptr);
        events::remove_rigidbody_event(goal);
    }
}
