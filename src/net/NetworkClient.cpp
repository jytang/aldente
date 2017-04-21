#include "NetworkClient.h"

NetworkClient::NetworkClient(boost::asio::io_service* ios) :
   io_service(ios), socket(*ios), connected(false) {
}

NetworkClient::~NetworkClient() {
    io_service->stop();
    service_thread.join();
}

bool NetworkClient::connect(std::string& host, unsigned int port) {
    tcp::resolver resolver(*io_service);
    tcp::resolver::query query(tcp::v4(), host, std::to_string(port));
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    try {
        boost::asio::connect(socket, endpoint_iterator);
    } catch (...) {
        return false;
    }
    service_thread = boost::thread(boost::bind(&NetworkClient::run_service, this));
    connected = true;
    return true;
}

bool NetworkClient::is_connected() const {
    return connected;
}

void NetworkClient::send(std::string& message) {
    boost::asio::write(socket, boost::asio::buffer(message));
}

bool NetworkClient::read_message(std::string& message) {
    if (message_queue.empty())
        return false;
    message = message_queue.pop();
    return true;
}

void NetworkClient::start_receive() {
    socket.async_receive(boost::asio::buffer(recv_buffer),
        boost::bind(&NetworkClient::handle_receive, this,
            boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void NetworkClient::handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (!error) {
        std::string message(recv_buffer.data(), recv_buffer.data() + bytes_transferred);
        message_queue.push(message);
        std::cerr << "[c] recv: " << message;
    }

    start_receive();
}

void NetworkClient::run_service() {
    start_receive();
    while (!io_service->stopped()) {
        try {
            io_service->run();
        }
        catch (const std::exception& e) {
            std::cerr << "Client network exception" << e.what();
        }
        catch (...) {
            std::cerr << "Unknown exception in client network";
        }
    }
}