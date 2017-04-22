#include "threadsafe_queue.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <ctime>
#include <mutex>
#include <map>

using boost::asio::ip::tcp;

/**
 * Represents one TCP connection to a client.
 */
class Connection : public boost::enable_shared_from_this<Connection> {
public:
    typedef boost::shared_ptr<Connection> pointer;

    // Initializes the socket.
    Connection(boost::asio::io_service& io_service) : socket(io_service) {}

    // Returns this connection's socket.
    tcp::socket& get_socket();

    // When connection starts, begin reading.
    void read_loop();

    // Sends a message to this client. Returns true if write was successful.
    bool send(string message);

    // Removes a message and returns it.
    bool read_message(string& message);

private:
    // Callback for when an asynchronous  read completes.
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);

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
    NetworkServer(boost::asio::io_service* ios, unsigned int port);

    // Sends a message to all clients.
    void send_to_all(string message);

    // Read all messages from all clients.
    // Returns a mapping of client id to list of messages.
    std::map<int, std::vector<string>> read_all_messages();

private:
    // Begin accepting new clients.
    void start_accept();

    // Callback for when a client is connected.
    void handle_accept(Connection::pointer new_connection,
        const boost::system::error_code& error);

    boost::asio::io_service* io_service;
    tcp::acceptor acceptor;
    std::map<int, Connection::pointer> client_list;
    mutex client_list_mutex;
    int next_id;
};
