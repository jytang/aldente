#include "network_manager.h"
#include "util/config.h"
#include "events.h"

#include <GLFW/glfw3.h>

NetworkServer* NetworkManager::server;
NetworkClient* NetworkManager::client;
bool NetworkManager::is_server;
string NetworkManager::server_host;
int NetworkManager::port;
boost::thread* NetworkManager::service_thread;
bool NetworkManager::is_connected = false;
boost::asio::io_service NetworkManager::io_service;

void NetworkManager::connect() {
    Config::config->get_value(Config::str_is_server, is_server);
    Config::config->get_value(Config::str_server_ip, server_host);
    Config::config->get_value(Config::str_port, port);

    server = is_server ? new NetworkServer(io_service, port) : nullptr;
    client = new NetworkClient(io_service);

    // Try establishing connection in separate thread.
    std::thread(&NetworkManager::attempt_connection).detach();
}

void NetworkManager::disconnect() {
    if (!io_service.stopped())
        io_service.stop();
    if (service_thread) {
        service_thread->join();
        delete service_thread;
    }
    delete server;
    delete client;
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
                service_thread = new boost::thread(&NetworkManager::run_service);
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
            std::cerr << "run_service: io_service error\n";
        }
    }
}

void NetworkManager::update() {
    if (server) {
        // std::unordered_map<int, std::vector<proto::ClientMessage>>
        auto recieved = server->read_all_messages();
        for (auto& sender : recieved) {
            for (auto& msg : sender.second) {
                switch (msg.message_type_case()) {
                    case proto::ClientMessage::MessageTypeCase::kBuildRequest: {
                        proto::Construct construct = msg.build_request();
                        events::build::ConstructData cd;
                        cd.type = static_cast<ConstructType>(construct.type());
                        cd.x = construct.x();
                        cd.z = construct.z();
                        events::build::try_build_event(cd);
                        std::cerr << "Fire try_build_event\n";
                    }
                }
            }
        }
    }
}
