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

void AldenteServer::start(bool is_windows) {
    Util::seed(0); // Seed PRNG.
    
    // For Visual Studio, we're running the server from the aldente/Server/ directory,
    // so we need Boost filesystem to go up one directory for the correct path.
    if (is_windows)
        boost::filesystem::current_path("..");
    
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
        network.update();
        GameState::update();
        scene_manager.get_current_scene()->update();

        Timer::get()->wait();
    }

    network.disconnect();
}
