#include "NetworkManager.h"
#include "util/config.h"

#include <GLFW/glfw3.h>

NetworkManager::NetworkManager() {
    Config::config->get_value(Config::str_is_server, is_server);
    Config::config->get_value(Config::str_server_ip, server_host);

    server = is_server ? new TcpServer(9000) : nullptr;
    client = new NetworkClient(server_host);
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
            is_connected = client->init();
            time_last_connect_attempt = glfwGetTime();
            if (is_connected) {
                std::cerr << "Established connection.\n";
            }
        }
    }
}