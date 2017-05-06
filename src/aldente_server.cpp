#include "aldente_server.h"

#include "util/config.h"
#include "game/game_state.h"
#include "physics.h"
#include "scene_manager.h"
#include "net/network_manager.h"
#include "timer.h"
#include <chrono>
#include <iostream>
#include <boost/filesystem.hpp>
#include "window.h"


AldenteServer::~AldenteServer() {
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


void AldenteServer::start() {
    Util::seed(0); // Seed PRNG.

    // For Visual Studio, we're running the server from the aldente/Server directory,
    // so we need to go up one directory for the correct path.
    #ifdef _WIN32
    boost::filesystem::current_path("..");
    #endif

    // Set up GLFW.
    assert(glfwInit());
    glfwSetErrorCallback([](int error, const char *description) {
        std::cerr << description << std::endl;
    });

    // Create hidden window, since GLEW needs a context.
    glfwWindowHint(GLFW_VISIBLE, false);
    Window hidden_window("hidden", true, 1280, 720);

    glSetup();
    ShaderManager::init();
    AssetLoader::setup();
    
    Physics physics;
    SceneManager scene_manager;

    // Game logic. Temporarily start game with build phase.
    GameState::init(&GameState::build_phase);

    MainScene testScene;
    physics.set_scene(&testScene);
    scene_manager.set_current_scene(&testScene);

    ServerNetworkManager network;
    network.connect();

    Timer timer(GAME_TICK);
    Timer::provide(&timer);

    Timer::get()->do_after(std::chrono::seconds(1), [](std::chrono::duration<double> d) {
        std::cout << "print me once after one second" << std::endl;
    });

    const std::function<void()> cancel = Timer::get()->do_every(
            std::chrono::milliseconds(500),
            [&cancel](std::chrono::duration<double> d) {
                static int count = 0;
                std::cout << "print me every 0.5 seconds: (" << ++count << " / 5)" << std::endl;
                if (count >= 5) cancel();
            });

    std::cerr << "Starting server..." << std::endl;

    while (true) {
        std::cerr << "BEFORE: " << Timer::get()->so_far().count() << std::endl;
        network.update();
        std::cerr << "NET: " << Timer::get()->so_far().count() << std::endl;
        GameState::update();
        std::cerr << "GS: " << Timer::get()->so_far().count() << std::endl;
        scene_manager.get_current_scene()->update();
        std::cerr << "SM: " << Timer::get()->so_far().count() << std::endl;

        Timer::get()->wait();
    }

    network.disconnect();
}
