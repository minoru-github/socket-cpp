#pragma once

#include <iostream>
#include <winsock2.h>
#include <string>

#pragma comment(lib,"Ws2_32.lib")

namespace communication {
    constexpr size_t recvbuflen = 512;

    class SocketTCP {
        SocketTCP();
        ~SocketTCP();

        char recvbuf[recvbuflen] = { 0 };
    public:
        void socket();
        SOCKET sock;
        const sockaddr_in create_ip_address();

        const std::string receive();
        void send(const std::string msg);
    };

    class Server : public SocketTCP {
    public:
        Server();
        ~Server();

        void bind();
        void listen();
        void accept();
    };

    class Client : public SocketTCP {
    public:
        Client();
        ~Client();

        void connect();
    };
} // namespace communication
