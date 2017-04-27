#include "aldente_client.h"

#include "window.h"
#include "asset_loader.h"
#include "physics.h"
#include "scene_manager.h"
#include "debug_input.h"
#include "poll/poller.h"
#include "poll/glfw_poller.h"
#include "poll/input_poller.h"
#include "util/config.h"
#include "events.h"
#include "ui/test_ui.h"
#include "render.h"
#include "game/phase.h"
#include "game/game_state.h"

AldenteClient::~AldenteClient() {
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

    TestUI ui = TestUI(5, 7, (float) width / (float) height);

    // Init the test scene.
    MainScene testScene;
    physics.set_scene(&testScene);
    scene_manager.set_current_scene(&testScene);
    DebugInput debug_input(window, scene_manager, physics);

    // Have window fire off a resize event to update all interested systems.
    window.broadcast_size();

    // Game logic.
    BuildPhase* bp = new BuildPhase();
    GameState::add_phase(PhaseType::BUILD, bp);
    GameState::set_phase(PhaseType::BUILD);

    events::joystick_event.connect([&](events::JoystickData d) {
        if (GameState::get_phase_type() == PhaseType::BUILD) {
            if (d.is_button == true && d.input == 0 && d.state == 0) {
                BuildPhase* p = dynamic_cast<BuildPhase*>(GameState::get_phase());
                p->is_menu = false;
            }
            else if (d.is_button == true && d.input == 1 && d.state == 0) {
                BuildPhase* p = dynamic_cast<BuildPhase*>(GameState::get_phase());
                p->is_menu = true;
            }
        }
    });

    while (!window.should_close()) {
        // Do polling
        for (auto &poller : pollers) {
            poller->poll();
        }

        debug_input.handle_movement();
        physics.update();

        scene_manager.get_current_scene()->update();

        render.update();
        ui.draw();
        window.swap_buffers();
    }
}
