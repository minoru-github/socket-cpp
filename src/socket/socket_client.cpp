#include "socket_client.h"

#include <iostream>
#include <winsock2.h>

// SocketClient::connect()内で<winsock2.h>内のconnet(...)を使おうとすると、
// SocketClient::connect()のconnectと名前が衝突してエラーになるので、
// new typeして回避
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
