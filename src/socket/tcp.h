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


    const int listen(
        const SOCKET listen_sock
    );
    const SOCKET accept(
        const SOCKET listen_sock
    );
    const int connect_to_server(
        const SOCKET sock
    );

    const int send_msg(
        const SOCKET sock,
        const std::string msg
    );
    const std::string receive_msg(
        const SOCKET sock
    );
} // namespace communication
