#include "network_server.h"

NetworkServer::NetworkServer(boost::asio::io_service& ios, unsigned int port) :
    acceptor(ios, tcp::endpoint(tcp::v4(), port)), next_id(0) {
    start_accept();
}

void NetworkServer::send_to_all(kuuhaku::proto::ServerMessage& message) {
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

std::unordered_map<int, std::vector<kuuhaku::proto::ClientMessage>> NetworkServer::read_all_messages() {
    std::unordered_map<int, std::vector<kuuhaku::proto::ClientMessage>> messages;
    unique_lock<mutex> lock(client_list_mutex);

    // No clients connected.
    if (client_list.empty()) {
        return messages;
    }

    for (auto const &c : client_list) {
        messages[c.first] = std::vector<kuuhaku::proto::ClientMessage>();
        string serialized;
        while ((serialized = c.second->read_message()).length() > 0) {
            kuuhaku::proto::ClientMessage message;
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
                unique_lock<mutex> lock(client_list_mutex);
                client_list[++next_id] = new_connection;
                new_connection->start_async_read_header();
            }
            else {
                std::cerr << "accept() error: " << error << "," << error.message() << "\n";
            }

            // Accept the next client.
            start_accept();
    });
}
