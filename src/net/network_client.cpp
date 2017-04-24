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
    connection.start_async_read_header();
    return true;
}

bool NetworkClient::is_connected() const {
    return connected;
}

bool NetworkClient::send(const google::protobuf::Message& message) {
    string serialized;
    message.SerializeToString(&serialized);
    return connection.send(serialized);
}
