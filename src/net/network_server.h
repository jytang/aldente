#pragma once

#include "connection.h"
#include "proto/net.pb.h"

#include <boost/bind.hpp>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <ctime>
#include <mutex>

using boost::asio::ip::tcp;

/**
 * Represents the single TCP server, managing many client connections.
 */
class NetworkServer {
public:
    // Initializes this server with the given port.
    NetworkServer(boost::asio::io_service& ios, unsigned int port);

    // Sends a message to all clients.
    void send_to_all(google::protobuf::Message& message);

    // Read all messages from all clients.
    // Returns a mapping of client id to list of messages.
    template <typename T>
    std::unordered_map<int, std::vector<T>> read_all_messages() {
        std::unordered_map<int, std::vector<T>> messages;
        unique_lock<mutex> lock(client_list_mutex);

        // No clients connected.
        if (client_list.empty()) {
            return messages;
        }

        for (auto const &c : client_list) {
            messages[c.first] = std::vector<T>();
            string serialized;
            while ((serialized = c.second->read_message()).length() > 0) {
                T message;
                message.SerializeToString(&serialized);
                messages[c.first].push_back(message);
            }
        }

        return messages;
    }

private:
    // Begin accepting new clients.
    void start_accept();

    boost::asio::io_service* io_service;
    tcp::acceptor acceptor;
    std::unordered_map<int, boost::shared_ptr<Connection>> client_list;
    mutex client_list_mutex;
    int next_id;
};
