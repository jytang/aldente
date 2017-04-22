#include "Connection.h"

tcp::socket& Connection::get_socket() {
    return socket;
}

void Connection::start_async_read_loop() {
    socket.async_read_some(boost::asio::buffer(rcvbuf),
        [&](const boost::system::error_code& error, size_t n_bytes) {
        if (!error) {
            message_queue.push(rcvbuf);
        } else if (error != boost::asio::error::eof) {
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

    // TODO (nice to have): do asynchronous write instead.
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