#pragma once

#include <assert.h>
#include <vector>
#include <string>

#define HEADER_SIZE 4

class Message {
public:
    // Decodes the header and returns the length of the message body.
    static uint32_t decode_header(const std::vector<uint8_t>& buf);

    // Encodes the header (size) and writes to the buffer.
    static void encode_header(std::vector<uint8_t>& buf, uint32_t size);
};