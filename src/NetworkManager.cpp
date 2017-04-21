#include "NetworkManager.h"
#include "util/config.h"

#include <GLFW/glfw3.h>

NetworkManager::NetworkManager() {
    Config::config->get_value(Config::str_is_server, is_server);
    Config::config->get_value(Config::str_server_ip, server_host);
    Config::config->get_value(Config::str_port, port);

    server = is_server ? new NetworkServer(&io_service, port) : nullptr;
    client = new NetworkClient(&io_service);
}

NetworkManager::~NetworkManager() {
    disconnect();
    delete server;
    delete client;
}

void NetworkManager::connect() {
    // Try establishing connection in separate thread.
    std::thread(&NetworkManager::attempt_connection, this).detach();
}

void NetworkManager::disconnect() {
    if (!io_service.stopped())
        io_service.stop();
    if (service_thread) {
        service_thread->join();
        delete service_thread;
    }
}

void NetworkManager::attempt_connection() {
    bool first_attempt = true;
    double time_last_connect_attempt = glfwGetTime();

    // Try connecting to the host every 5 seconds.
    while (!is_connected) {
        if (first_attempt || glfwGetTime() - time_last_connect_attempt > CONN_RETRY_SEC) {
            first_attempt = false;
            std::cerr << "Attempting to connect to " << server_host << "...\n";
            is_connected = client->connect(server_host, port);
            time_last_connect_attempt = glfwGetTime();
            if (is_connected) {
                std::cerr << "Established connection.\n";
                service_thread = new boost::thread(&NetworkManager::run_service, this);
            }
        }
    }
}

void NetworkManager::run_service() {
    while (service_thread && !io_service.stopped()) {
        try {
            io_service.poll();
        } catch (...) {
            // Silently ignore errors rather than crash. Shouldn't happen.
            std::cerr << "io_service error\n";
        }
    }
}