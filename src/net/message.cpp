#include "message.h"

uint32_t Message::decode_header(const std::vector<uint8_t>& buf) {
    if (buf.size() < HEADER_SIZE)
        return 0;
    uint32_t msg_size = 0;
    for (uint32_t i = 0; i < HEADER_SIZE; ++i)
        msg_size = msg_size * 256 + (static_cast<unsigned int>(buf[i]) & 0xFF);
    return msg_size;
}

void Message::encode_header(std::vector<uint8_t>& buf, uint32_t size) {
    assert(buf.size() >= HEADER_SIZE);
    buf[0] = static_cast<uint8_t>((size >> 24) & 0xFF);
    buf[1] = static_cast<uint8_t>((size >> 16) & 0xFF);
    buf[2] = static_cast<uint8_t>((size >>  8) & 0xFF);
    buf[3] = static_cast<uint8_t>((size)       & 0xFF);
}