#include "connection.h"

tcp::socket& Connection::get_socket() {
    return socket;
}

void Connection::start_async_read_header() {
    rcvbuf.resize(HEADER_SIZE);
    socket.async_read_some(boost::asio::buffer(rcvbuf),
        [&](const boost::system::error_code& error, size_t n_bytes) {
        if (error && error != boost::asio::error::eof) {
            std::cerr << "ERROR: could not read header: " << error << "\n";
            return;
        }

        // Read the message body.
        uint32_t length = Message::decode_header(rcvbuf);
        start_async_read_body(length);
    });
}

void Connection::start_async_read_body(uint32_t length) {
    rcvbuf.resize(HEADER_SIZE + length);
    socket.async_read_some(boost::asio::buffer(&rcvbuf[HEADER_SIZE], length),
        [&, length](const boost::system::error_code& error, size_t n_bytes) {
        if (!error) {
            if (length != n_bytes) {
                std::cerr << "ERROR: received unexpected num of bytes; dropping message\n";
            } else {
                std::string body(rcvbuf.begin() + HEADER_SIZE, rcvbuf.end());
                cerr << "[con] got message: " << body;
                message_queue.push(body);
            }
        } else if (error != boost::asio::error::eof) {
            std::cerr << "ERROR: could not read body: " << error << "\n";
            return;
        }

        // Read the header of the next message.
        start_async_read_header();
    });
}

bool Connection::send(const string& message) {
    if (!socket.is_open()) {
        return false;
    }

    uint8_t msg_size = message.length();

    std::vector<uint8_t> sndbuf;
    sndbuf.resize(HEADER_SIZE + msg_size);
    Message::encode_header(sndbuf, msg_size);
    for (int i = 0; i < msg_size; i++) {
        sndbuf[HEADER_SIZE + i] = message[i];
    }

    // TODO (nice to have): do asynchronous write instead.
    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer(sndbuf), error);

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
