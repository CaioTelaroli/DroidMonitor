#include "Protocol.hpp"

#include <winsock2.h>
#include <ws2tcpip.h>

#include <array>
#include <cstring>
#include <limits>
#include <climits>

namespace dm {

static bool sendAll(
    SOCKET s,
    const char* data,
    size_t size
) {
    while (size > 0) {
        int chunk = static_cast<int>(
            size > INT_MAX ? INT_MAX : size
        );

        int sent = send(s, data, chunk, 0);

        if (sent == SOCKET_ERROR || sent == 0)
            return false;

        data += sent;
        size -= static_cast<size_t>(sent);
    }

    return true;
}

static bool receiveAll(
    SOCKET s,
    char* data,
    size_t size
) {
    while (size > 0) {
        int chunk = static_cast<int>(
            size > INT_MAX ? INT_MAX : size
        );

        int received = recv(s, data, chunk, 0);

        if (received <= 0)
            return false;

        data += received;
        size -= static_cast<size_t>(received);
    }

    return true;
}

bool sendMessage(
    uintptr_t socket,
    MessageType type,
    std::string_view payload
) {
    if (payload.size() > MAX_PAYLOAD)
        return false;

    SOCKET s = static_cast<SOCKET>(socket);

    std::array<char, 12> header{};

    std::memcpy(header.data(), "DMON", 4);

    auto write16 = [&](int offset, uint16_t value) {
        header[offset] = static_cast<char>(value >> 8);
        header[offset + 1] = static_cast<char>(value);
    };

    auto write32 = [&](int offset, uint32_t value) {
        header[offset] = static_cast<char>(value >> 24);
        header[offset + 1] = static_cast<char>(value >> 16);
        header[offset + 2] = static_cast<char>(value >> 8);
        header[offset + 3] = static_cast<char>(value);
    };

    write16(4, VERSION);
    write16(6, static_cast<uint16_t>(type));
    write32(8, static_cast<uint32_t>(payload.size()));

    if (!sendAll(s, header.data(), header.size()))
        return false;

    if (!payload.empty()) {
        return sendAll(
            s,
            payload.data(),
            payload.size()
        );
    }

    return true;
}

bool receiveMessage(
    uintptr_t socket,
    Message& message
) {
    SOCKET s = static_cast<SOCKET>(socket);

    std::array<char, 12> header{};

    if (!receiveAll(s, header.data(), header.size()))
        return false;

    if (std::memcmp(header.data(), "DMON", 4) != 0)
        return false;

    auto read16 = [&](int offset) -> uint16_t {
        return
            (static_cast<uint16_t>(
                static_cast<unsigned char>(header[offset])
            ) << 8) |
            static_cast<unsigned char>(header[offset + 1]);
    };

    auto read32 = [&](int offset) -> uint32_t {
        return
            (static_cast<uint32_t>(
                static_cast<unsigned char>(header[offset])
            ) << 24) |
            (static_cast<uint32_t>(
                static_cast<unsigned char>(header[offset + 1])
            ) << 16) |
            (static_cast<uint32_t>(
                static_cast<unsigned char>(header[offset + 2])
            ) << 8) |
            static_cast<unsigned char>(header[offset + 3]);
    };

    const uint16_t version = read16(4);
    const uint16_t type = read16(6);
    const uint32_t length = read32(8);

    if (version != VERSION || length > MAX_PAYLOAD)
        return false;

    message.type = static_cast<MessageType>(type);
    message.payload.resize(length);

    if (length > 0) {
        if (!receiveAll(s, message.payload.data(), length))
            return false;
    }

    return true;
}

}