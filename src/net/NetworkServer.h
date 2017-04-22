#include "threadsafe_queue.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <unordered_map>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <ctime>
#include <mutex>

using boost::asio::ip::tcp;

/**
 * Represents one TCP connection to a client.
 */
class Connection : public boost::enable_shared_from_this<Connection> {
public:
    // Initializes the socket.
    Connection(boost::asio::io_service& io_service) : socket(io_service) {}

    // Returns this connection's socket.
    tcp::socket& get_socket();

    // When connection starts, begin reading.
    void start_async_read_loop();

    // Sends a message to this client. Returns true if write was successful.
    bool send(const string& message);

    // Removes a message and returns it.
    bool read_message(string* message);

private:
    tcp::socket socket;
    char rcvbuf[BUFSIZ];
    ThreadSafeQueue<string> message_queue;
};

/**
 * Represents the single TCP server, managing many client connections.
 */
class NetworkServer {
public:
    // Initializes this server with the given port.
    NetworkServer(boost::asio::io_service& ios, unsigned int port);

    // Sends a message to all clients.
    void send_to_all(string message);

    // Read all messages from all clients.
    // Returns a mapping of client id to list of messages.
    std::unordered_map<int, std::vector<string>> read_all_messages();

private:
    // Begin accepting new clients.
    void start_accept();

    boost::asio::io_service* io_service;
    tcp::acceptor acceptor;
    std::unordered_map<int, boost::shared_ptr<Connection>> client_list;
    mutex client_list_mutex;
    int next_id;
};
