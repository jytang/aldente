#include "aldente.h"

#include "window.h"
#include "asset_loader.h"
#include "physics.h"
#include "shadows.h"
#include "scene_manager.h"
#include "debug_input.h"
#include "poll/poller.h"
#include "poll/glfw_poller.h"
#include "poll/input_poller.h"
#include "util/config.h"
#include "net/network_manager.h"
#include "events.h"
#include "render.h"

Aldente::~Aldente() {
    ShaderManager::destroy();
    GeometryGenerator::destroy();
    glfwTerminate();
}

static void glSetup() {
    assert(glewInit() == GLEW_OK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Aldente::start_game_loop() {
    Util::seed(0); // Seed PRNG.

    // Set up GLFW.
    assert(glfwInit());
    glfwSetErrorCallback([](int error, const char *description) {
        std::cerr << description << std::endl;
    });

    // Create a window.
    int width, height;
    Config::config->get_value(Config::str_screen_width, width);
    Config::config->get_value(Config::str_screen_height, height);
    std::string game_name;
    Config::config->get_value(Config::str_game_name, game_name);
    Window window(game_name, true, width, height);

    // Setup subsystems after window creation.
    glSetup();
    ShaderManager::init();
    AssetLoader::asset_loader->setup();

    // Set up list of polling objects.
    std::vector<std::shared_ptr<Poller>> pollers {
            std::make_shared<GlfwPoller>(),
            std::make_shared<InputPoller>(),
    };

	Physics physics;
    SceneManager scene_manager;
	Render render(window, scene_manager);

	// Init the test scene.
	MainScene testScene;
	physics.set_scene(&testScene);
	scene_manager.set_current_scene(&testScene);
    DebugInput debug_input(window, scene_manager, physics);

    NetworkManager network;
    network.connect();

    while (!window.should_close()) {
        // Do polling
        for (auto &poller : pollers) {
            poller->poll();
        }

        if (network.get_server()) {
            kuuhaku::proto::GameObject msg;
            msg.set_type(kuuhaku::proto::GameObject_Type::GameObject_Type_TILE);
            auto location = new kuuhaku::proto::GameObject_Location();
            location->add_x(420);
            msg.set_allocated_location(location);
            std::cerr << "[aldente] sending msg: " << msg.DebugString();
            network.get_server()->send_to_all(msg);

            kuuhaku::proto::GameObject rcv_msg;
            while (network.get_client()->template read_message<kuuhaku::proto::GameObject>(&rcv_msg))
                std::cerr << "[aldente] parsed msg: " << rcv_msg.DebugString();
        }

        debug_input.handle_movement();
        physics.update();

        scene_manager.get_current_scene()->update();

		render.update();
    }
}
