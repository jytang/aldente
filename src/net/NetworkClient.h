/**
* Source: http://codereview.stackexchange.com/questions/51235/udp-network-server-client-for-gaming-using-boost-asio
* License: MIT
*/

#include "threadsafe_queue.h"
#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include <array>

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
    bool send(const string& message);

    // Removes and returns a message from the FIFO queue.
    bool read_message(string* message);

private:
    // Begin receiving messages by adding an async receive task.
    void start_receive();

    tcp::resolver resolver;
    tcp::socket socket;
    boost::array<char, BUFSIZ> recv_buffer;
    ThreadSafeQueue<string> message_queue;
    bool connected;
};
