#pragma once

#include "threadsafe_queue.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#include <string>

#define HEADER_SIZE 4

using boost::asio::ip::tcp;

/**
* Represents one TCP connection to a client.
*/
class Connection : public boost::enable_shared_from_this<Connection> {
public:
    Connection();

    // Initializes the socket.
    Connection(boost::asio::io_service& io_service) : socket(io_service) {}

    // Returns this connection's socket.
    tcp::socket& get_socket();

    // When connection starts, begin reading header.
    void start_async_read_header();

    // When connection starts, begin reading body.
    void start_async_read_body(uint32_t length);

    // Sends a message to this client. Returns true if write was successful.
    bool send(const string& message);

    // Removes a message and returns it. Empty string means there are no messages.
    string read_message();

private:
    // Encodes the 32-bit size and writes to the buffer as 4 bytes.
    static void encode_header(std::vector<uint8_t>& buf, uint32_t size);

    // Decodes the header and returns the length of the message body as 32-bit int.
    static uint32_t decode_header(const std::vector<uint8_t>* buf);

    tcp::socket socket;
    std::vector<uint8_t> rcvbuf;
    ThreadSafeQueue<string> message_queue;
};