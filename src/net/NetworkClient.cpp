#include "NetworkClient.h"

NetworkClient::NetworkClient(boost::asio::io_service* ios) :
    io_service(ios), socket(*ios), connected(false) {
}

bool NetworkClient::connect(string& host, unsigned int port) {
    tcp::resolver resolver(*io_service);
    tcp::resolver::query query(tcp::v4(), host, std::to_string(port));
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    try {
        boost::asio::connect(socket, endpoint_iterator);
    } catch (...) {
        return false;
    }

    connected = true;
    start_receive();
    return true;
}

bool NetworkClient::is_connected() const {
    return connected;
}

bool NetworkClient::send(string& message) {
    if (!connected)
        return false;
    boost::asio::write(socket, boost::asio::buffer(message));
    return true;
}

bool NetworkClient::read_message(string& message) {
    if (message_queue.empty())
        return false;
    message = message_queue.pop();
    return true;
}

void NetworkClient::start_receive() {
    socket.async_receive(
        boost::asio::buffer(recv_buffer),
        [&](const boost::system::error_code &error, size_t n_bytes) {
            if (!error) {
                string message(recv_buffer.data(),
                               recv_buffer.data() + n_bytes);
                message_queue.push(message);

                // TODO logging framework
                std::cerr << "[c] recv: " << message;
            }
            start_receive();
        });
}
