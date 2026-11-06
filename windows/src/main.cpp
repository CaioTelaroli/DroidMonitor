#include "TcpClient.hpp"

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::cout << "=============================\n";
    std::cout << "      DroidMonitor Host\n";
    std::cout << "=============================\n\n";

    if (argc < 2) {
        std::cout << "Uso:\n";
        std::cout << "DroidMonitorHost.exe IP_ANDROID\n";
        std::cout << "\nExemplo:\n";
        std::cout << "DroidMonitorHost.exe 192.168.42.129\n";
        return 1;
    }

    dm::TcpClient client;

    if (!client.connectTo(argv[1], 24816)) {
        std::cerr << "Falha na conexao.\n";
        return 1;
    }

    if (!client.handshake()) {
        std::cerr << "Falha no handshake.\n";
        client.disconnect();
        return 1;
    }

    std::cout << "\nDispositivo Android conectado!\n";
    std::cout << "Pressione ENTER para desconectar.\n";

    std::string input;
    std::getline(std::cin, input);

    client.disconnect();

    return 0;
}