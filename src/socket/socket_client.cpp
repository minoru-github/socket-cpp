#include "socket_client.h"

#include <iostream>

auto winsock2_connect = connect;

namespace communication
{

    SocketClient::SocketClient()
    {
    }

    SocketClient::~SocketClient()
    {
    }

    void SocketClient::connect()
    {
        const auto addr = create_ip_address();

        const int error = winsock2_connect(this->sock, (sockaddr*)&addr, static_cast<int>(sizeof(addr)));
        if (error) {
            std::cerr << "can't connect. error: " + std::to_string(WSAGetLastError()) << std::endl;
            //finalize_socket_communication(sock);
        }
    }

} // namespace communication
