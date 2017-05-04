#include "aldente_client.h"

#include "window.h"
#include "input/conceptual_translator.h"
#include "input/axis_combiner.h"
#include "input/raw_maps/xbox.h"
#include "input/raw_maps/matricom.h"
#include "input/raw_maps/debug.h"
#include "asset_loader.h"
#include "physics.h"
#include "scene_manager.h"
#include "debug_input.h"
#include "poll/poller.h"
#include "poll/glfw_poller.h"
#include "poll/input_poller.h"
#include "util/config.h"
#include "events.h"
#include "ui/build_ui.h"
#include "render.h"
#include "game/game_state.h"
#include "game/construct_types.h"
#include "net/network_manager.h"
#include "shaders/shader_manager.h"

AldenteClient::~AldenteClient() {
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void AldenteClient::start() {
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

    // Create input handlers.
    input::ConceptualTranslator translator(input::BTN_MAP_MATRICOM, input::KBD_MAP_DEBUG);
    input::AxisCombiner stick_handler(translator, input::STICKS_DEFAULT);

    // Setup subsystems after window creation.
    glSetup();
    ShaderManager::init();
    AssetLoader::setup();

    // Set up list of polling objects.
    std::vector<std::shared_ptr<Poller>> pollers {
        std::make_shared<GlfwPoller>(),
        std::make_shared<InputPoller>(),
    };

    Physics physics;
    SceneManager scene_manager;
    Render render(window, scene_manager);

    std::vector<ConstructData> constructs;
    for (int i = 0; i < 12; i++) {
        if (i % 2 == 0)
            constructs.push_back(Constructs::CHEST);
        else
            constructs.push_back(Constructs::REMOVE);
    }
    BuildUI ui = BuildUI(3, 4, (float) width / (float) height, constructs);

    // Init the test scene.
    MainScene testScene;
    physics.set_scene(&testScene);
    scene_manager.set_current_scene(&testScene);
    DebugInput debug_input(window, scene_manager, physics);

    // Have window fire off a resize event to update all interested systems.
    window.broadcast_size();

    // Game logic. Temporarily start game with build phase.
    GameState::init(&GameState::build_phase);

    bool is_server;
    Config::config->get_value(Config::str_is_server, is_server);
    NetworkManager::connect(is_server);

    while (!window.should_close()) {
        // Do polling
        for (auto &poller : pollers) {
            poller->poll();
        }

        NetworkManager::update();
        GameState::update();

        debug_input.handle_movement();
        physics.update();

        scene_manager.get_current_scene()->update();

        render.update();
        ui.draw();
        window.swap_buffers();
    }

    NetworkManager::disconnect();
}
