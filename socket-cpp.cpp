#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib,"Ws2_32.lib")

int main()
{
    WSADATA data{};
    const auto err_startup = WSAStartup(MAKEWORD(2, 0), &data);
    if (err_startup) {
        std::cerr << "can't startup. error: " + std::to_string(err_startup);
        return err_startup;
    }

    const char* hostname = "localhost";
    const char* service = "80";
    addrinfo hints{};
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;

    addrinfo* res = nullptr;
    const auto err_getaddrinfo = getaddrinfo(hostname, service, &hints, &res);
    if (err_getaddrinfo) {
        std::cerr << "can't getaddrinfo. error: " + std::to_string(err_getaddrinfo);
        return err_getaddrinfo;
    }

    const SOCKET sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == INVALID_SOCKET) {
        std::cerr << "can't create socket.";
        return 1;
    }

    const auto err_connect = connect(sock, res->ai_addr, static_cast<int>(res->ai_addrlen));
    if (err_connect) {
        std::cerr << "can't connect. error: " + std::to_string(err_connect);
        return err_connect;
    }

    freeaddrinfo(res);
    WSACleanup();

    return 0;
}
