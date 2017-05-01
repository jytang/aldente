#include "network_client.h"

NetworkClient::NetworkClient(boost::asio::io_service& ios) :
    connection(ios), resolver(ios), connected(false) {
}

bool NetworkClient::connect(const string& host, unsigned int port) {
    tcp::resolver::query query(tcp::v4(), host, std::to_string(port), 
        boost::asio::ip::resolver_query_base::flags());
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

bool NetworkClient::send(const proto::ClientMessage& message) {
    string serialized;
    message.SerializeToString(&serialized);
    return connection.send(serialized);
}

bool NetworkClient::read_message(proto::ServerMessage* message) {
    string serialized = connection.read_message();
    if (serialized.length() == 0)
        return false;
    return message->ParseFromString(serialized);
}
