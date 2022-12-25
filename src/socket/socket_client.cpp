#include "socket_client.h"

#include <iostream>
#include <winsock2.h>

// SocketClient::connect()����<winsock2.h>����connet(...)���g�����Ƃ���ƁA
// SocketClient::connect()��connect�Ɩ��O���Փ˂��ăG���[�ɂȂ�̂ŁA
// new type���ĉ��
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
