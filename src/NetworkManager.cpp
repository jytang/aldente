#include "NetworkManager.h"
#include "util/config.h"

#include <GLFW/glfw3.h>

NetworkManager::NetworkManager() {
    Config::config->get_value(Config::str_is_server, is_server);
    Config::config->get_value(Config::str_server_ip, server_host);
    Config::config->get_value(Config::str_port, port);

    server = is_server ? new NetworkServer(port) : nullptr;
    client = new NetworkClient(&io_service);

    //std::thread(&NetworkManager::run_service, this).detach();
}

void NetworkManager::connect() {
    // Repeatedly try establishing connection in separate thread.
    std::thread(&NetworkManager::attempt_connection, this).detach();
}

void NetworkManager::attempt_connection() {
    bool first_attempt = true;
    double time_last_connect_attempt = glfwGetTime();

    // Try connecting to the host every 5 seconds.
    while (!is_connected) {
        if (first_attempt || glfwGetTime() - time_last_connect_attempt > 5.f) {
            first_attempt = false;
            std::cerr << "Attempting to connect to " << server_host << "...\n";
            is_connected = client->connect(server_host, port);
            time_last_connect_attempt = glfwGetTime();
            if (is_connected) {
                std::cerr << "Established connection.\n";
            }
        }
    }
}

void NetworkManager::run_service() {
    while (!io_service.stopped()) {
        try {
            io_service.run();
        } catch (...) {
            std::cerr << "io_service error\n";
        }
    }
}