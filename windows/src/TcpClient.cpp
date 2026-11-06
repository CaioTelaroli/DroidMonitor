#include "TcpClient.hpp"
#include "Protocol.hpp"

#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>

namespace dm {

bool TcpClient::connectTo(
    const std::string& ip,
    unsigned short port
) {
    WSADATA data{};

    if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
        return false;

    SOCKET s = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    if (s == INVALID_SOCKET)
        return false;

    sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    if (inet_pton(
        AF_INET,
        ip.c_str(),
        &address.sin_addr
    ) != 1) {
        closesocket(s);
        return false;
    }

    std::cout << "Conectando a " << ip << ":" << port << "\n";

    if (connect(
        s,
        reinterpret_cast<sockaddr*>(&address),
        sizeof(address)
    ) == SOCKET_ERROR) {
        closesocket(s);
        return false;
    }

    socket_ = static_cast<uintptr_t>(s);

    std::cout << "Conexao TCP estabelecida!\n";

    return true;
}

bool TcpClient::handshake() {
    if (socket_ == static_cast<uintptr_t>(-1))
        return false;

    if (!sendMessage(
        socket_,
        MessageType::HELLO,
        "DROIDMONITOR_HOST"
    )) {
        return false;
    }

    Message response{};

    if (!receiveMessage(socket_, response))
        return false;

    if (response.type != MessageType::HELLO_ACK)
        return false;

    if (response.payload != "DROIDMONITOR_ANDROID")
        return false;

    std::cout << "Handshake concluido com sucesso!\n";

    return true;
}

void TcpClient::disconnect() {
    if (socket_ == static_cast<uintptr_t>(-1))
        return;

    SOCKET s = static_cast<SOCKET>(socket_);

    sendMessage(
        socket_,
        MessageType::DISCONNECT,
        "HOST_SHUTDOWN"
    );

    shutdown(s, SD_BOTH);
    closesocket(s);

    socket_ = static_cast<uintptr_t>(-1);

    WSACleanup();
}

}