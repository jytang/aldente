#include "network_manager.h"
#include "util/config.h"
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
                register_listeners();
                service_thread = new boost::thread(&NetworkManager::run_service);
            }
        }
    }
}

void NetworkManager::register_listeners() {
    // Build phase.
    events::build::request_build_event.connect([](events::build::ConstructData& cd) {
        proto::ClientMessage msg;
        proto::Construct* construct = new proto::Construct();
        to_construct(cd, construct);
        msg.set_allocated_build_request(construct);
        client->send(msg);
    });

    events::build::respond_build_event.connect([](events::build::ConstructData& cd, bool permitted) {
        proto::ServerMessage msg;
        msg.set_status(permitted);
        proto::Construct* construct = new proto::Construct();
        to_construct(cd, construct);
        msg.set_allocated_build_update(construct);
        server->send_to_all(msg);
    });
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
    if (server)
        update_server();
    if (client)
        update_client();
}

void NetworkManager::update_server() {
    for (auto& client : server->read_all_messages()) {
        for (auto& msg : client.second) {
            switch (msg.message_type_case()) {
            case proto::ClientMessage::MessageTypeCase::kBuildRequest: {
                proto::Construct construct = msg.build_request();
                events::build::ConstructData cd;
                to_construct_data(construct, cd);
                events::build::try_build_event(cd);
                break;
            }
            default:
                break;
            }
        }
    }
}

void NetworkManager::update_client() {
    proto::ServerMessage msg;
    while (client->read_message(&msg)) {
        switch (msg.message_type_case()) {
        case proto::ServerMessage::MessageTypeCase::kBuildUpdate: {
            if (!msg.status()) {
                // TODO: construct placement failed. notify player?
                break;
            }

            proto::Construct construct = msg.build_update();
            events::build::ConstructData cd;
            to_construct_data(construct, cd);
            events::build::update_build_event(cd);
        }
        default:
            break;
        }
    }
}

void NetworkManager::to_construct(events::build::ConstructData& cd, proto::Construct* c) {
    c->set_type(cd.type);
    c->set_x(cd.x);
    c->set_z(cd.z);
}

void NetworkManager::to_construct_data(proto::Construct& c, events::build::ConstructData& cd) {
    cd.type = static_cast<ConstructType>(c.type());
    cd.x = c.x();
    cd.z = c.z();
}

