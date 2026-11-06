#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace dm {

constexpr uint16_t VERSION = 1;
constexpr uint16_t PORT = 24816;
constexpr uint32_t MAX_PAYLOAD = 1024 * 1024;

enum class MessageType : uint16_t {
    HELLO = 1,
    HELLO_ACK = 2,
    PING = 3,
    PONG = 4,
    DISCONNECT = 5,
    VIDEO_CONFIG = 10,
    VIDEO_FRAME = 11,
    VIDEO_ACK = 12
};

struct Message {
    MessageType type;
    std::string payload;
};

bool sendMessage(
    uintptr_t socket,
    MessageType type,
    std::string_view payload
);

bool receiveMessage(
    uintptr_t socket,
    Message& message
);

}