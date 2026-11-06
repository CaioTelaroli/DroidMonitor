#pragma once

#include <string>

namespace dm {

class TcpClient {
public:
    bool connectTo(
        const std::string& ip,
        unsigned short port
    );

    bool handshake();

    void disconnect();

private:
    uintptr_t socket_ = static_cast<uintptr_t>(-1);
};

}