#include "network_client.h"

NetworkClient::NetworkClient(boost::asio::io_service& ios) :
    connection(ios), resolver(ios), connected(false) {
}

bool NetworkClient::connect(const string& host, unsigned int port) {
    tcp::resolver::query query(tcp::v4(), host, std::to_string(port));
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    try {
        boost::asio::connect(connection.get_socket(), endpoint_iterator);
    } catch (...) {
        return false;
    }

    connected = true;
    connection.start_async_read_loop();
    return true;
}

bool NetworkClient::is_connected() const {
    return connected;
}

bool NetworkClient::send(const string& message) {
    return connection.send(message);
}

bool NetworkClient::read_message(string* message) {
    return connection.read_message(message);
}
