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
    void send_to_all(proto::ServerMessage& message);

    // Read all messages from all clients.
    // Returns a mapping of client id to list of messages.
    std::unordered_map<int, std::vector<proto::ClientMessage>> read_all_messages();

private:
    // Begin accepting new clients.
    void start_accept();

    boost::asio::io_service* io_service;
    tcp::acceptor acceptor;
    std::unordered_map<int, boost::shared_ptr<Connection>> client_list;
    mutex client_list_mutex;
    int next_id;
};
