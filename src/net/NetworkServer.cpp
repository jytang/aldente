#include "NetworkServer.h"

tcp::socket& Connection::get_socket() {
    return socket;
}

void Connection::start_async_read_loop() {
    socket.async_read_some(boost::asio::buffer(rcvbuf),
        [&](const boost::system::error_code& error, size_t n_bytes) {
            if (!error) {
                message_queue.push(rcvbuf);
                std::cerr << "[s] recv: " << string(rcvbuf);
            }
            else if (error != boost::asio::error::eof) {
                std::cerr << "FATAL handle_read error: " << error << "\n";
                return;
            }

            // Wait for and read the next message.
            start_async_read_loop();
        });
}

bool Connection::send(const string& message) {
    if (!socket.is_open()) {
        return false;
    }

    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer(message), error);

    // If we get these errors, it's likely a clean disconnect.
    if ((error == boost::asio::error::eof) ||
        (error == boost::asio::error::connection_reset) ||
        (error == boost::asio::error::broken_pipe)) {
        std::cerr << "[send] client disconnected.\n";
        return false;
    } else if (error) {
        std::cerr << "[send] some other error: " << error << "\n";
        return false;
    }

    return true;
}

bool Connection::read_message(string* message) {
    if (message_queue.empty())
        return false;
    *message = message_queue.pop();
    return true;
}

NetworkServer::NetworkServer(boost::asio::io_service& ios, unsigned int port) :
    acceptor(ios, tcp::endpoint(tcp::v4(), port)), next_id(0) {
    start_accept();
}

void NetworkServer::send_to_all(string message) {
    unique_lock<mutex> lock(client_list_mutex);

    // No clients connected.
    if (client_list.empty()) {
        return;
    }

    for (auto c = client_list.begin(); c != client_list.end(); /* empty */) {
        bool success = c->second->send(message);
        if (!success) {
            std::cerr << "Write failed!\n";
            // If write failed, it's likely because of disconnect. Remove from clients.
            c = client_list.erase(c);
        } else {
            c++;
        }
    }
}

std::unordered_map<int, std::vector<string>> NetworkServer::read_all_messages() {
    std::unordered_map<int, std::vector<string>> messages;
    unique_lock<mutex> lock(client_list_mutex);

    // No clients connected.
    if (client_list.empty()) {
        return messages;
    }

    for (auto const &c : client_list) {
        messages[c.first] = std::vector<string>();
        string message;
        while (c.second->read_message(&message)) {
            messages[c.first].push_back(message);
        }
    }

    return messages;
}

void NetworkServer::start_accept() {
    auto new_connection = boost::shared_ptr<Connection>(
        new Connection(acceptor.get_io_service()));

    acceptor.async_accept(new_connection->get_socket(),
        [&, new_connection](const boost::system::error_code& error) {
            if (!error) {
                std::cerr << "Accepted new connection." << std::endl;
                unique_lock<mutex> lock(client_list_mutex);
                client_list[++next_id] = new_connection;
                new_connection->start_async_read_loop();
            }
            else {
                std::cerr << "accept() error: " << error << "," << error.message() << "\n";
            }

            // Accept the next client.
            start_accept();
    });
}
