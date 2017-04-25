/**
* Source: http://codereview.stackexchange.com/questions/51235/udp-network-server-client-for-gaming-using-boost-asio
* License: MIT
*/

#pragma once

#include "connection.h"
#include "proto/net.pb.h"

using boost::asio::ip::tcp;

/**
* Represents a single client on the network.
*/
class NetworkClient {
public:
    NetworkClient(boost::asio::io_service& ios);

    // Synchronously tries to connect to host. Returns true if successful.
    bool connect(const string& host, unsigned int port);

    // Returns true if client was successfully connected.
    bool is_connected() const;

    // Synchronously sends a message to the server. Returns true if successful.
    bool send(const kuuhaku::proto::ClientMessage& message);

    // Removes and returns a message from the FIFO queue.
    bool read_message(kuuhaku::proto::ServerMessage* message);

private:
    Connection connection;
    tcp::resolver resolver;
    bool connected;
};
