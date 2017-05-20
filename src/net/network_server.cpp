#include "network_server.h"
#include "events.h"

NetworkServer::NetworkServer(boost::asio::io_service& ios, unsigned int port) :
    acceptor(ios, tcp::endpoint(tcp::v4(), port)) {
    start_accept();
}

void NetworkServer::send_to_all(proto::ServerMessage& message) {
    unique_lock<mutex> lock(client_list_mutex);

    // No clients connected.
    if (client_list.empty()) {
        return;
    }

    string serialized;
    for (auto c = client_list.begin(); c != client_list.end(); /* empty */) {
        message.SerializeToString(&serialized);
        bool success = c->second->send(serialized);
        if (!success) {
            std::cerr << "Write failed!\n";
            // If write failed, it's likely because of disconnect. Remove from clients.
            c = client_list.erase(c);
        }
        else {
            c++;
        }
    }
}

void NetworkServer::send_to(int id, proto::ServerMessage& message) {
    unique_lock<mutex> lock(client_list_mutex);
    if (client_list.find(id) == client_list.end()) {
        std::cerr << "Write failed! Couldn't write to client " << id << std::endl;
        return;
    }
    string serialized;
    message.SerializeToString(&serialized);
    bool success = client_list[id]->send(serialized);
    if (!success) {
        // If write failed, it's likely because of disconnect. Remove from clients.
        std::cerr << "Write failed! Client likely disconnected." << std::endl;
        client_list.erase(id);
    }
}


std::unordered_map<int, std::vector<proto::ClientMessage>> NetworkServer::read_all_messages() {
    std::unordered_map<int, std::vector<proto::ClientMessage>> messages;
    unique_lock<mutex> lock(client_list_mutex);

    // No clients connected.
    if (client_list.empty()) {
        return messages;
    }

    for (auto const &c : client_list) {
        messages[c.first] = std::vector<proto::ClientMessage>();
        string serialized;
        while ((serialized = c.second->read_message()).length() > 0) {
            proto::ClientMessage message;
            message.ParseFromString(serialized);
            messages[c.first].push_back(message);
        }
    }

    return messages;
}

void NetworkServer::start_accept() {
    auto new_connection = boost::shared_ptr<Connection>(
        new Connection(acceptor.get_io_service()));

    acceptor.async_accept(new_connection->get_socket(),
        [&, new_connection](const boost::system::error_code& error) {
            if (!error) {
                std::cerr << "Accepted new connection." << std::endl;
                int next_id = next_available_id();
                {
                    unique_lock<mutex> lock(client_list_mutex);
                    client_list[next_id] = new_connection;
                }
                new_connection->start_async_read_header();
                events::menu::request_join_event(next_id);
            }
            else {
                std::cerr << "accept() error: " << error << "," << error.message() << "\n";
            }

            // Accept the next client.
            start_accept();
    });
}

int NetworkServer::next_available_id() {
    int id = 1;
    while (client_list.find(id) != client_list.end()) {
        id++;
    }
    return id;
}
