#include "socket_server.h"

#include <iostream>
#include <winsock2.h>

// SocketServer::bind()内で<winsock2.h>内のbind(...)を使おうとすると、
// SocketServer::bind()のbindと名前が衝突してエラーになるので、
// new typeして回避。
// その他も同様。
auto winsock2_bind = bind;
auto winsock2_litsen = listen;
auto winsock2_accept = accept;

namespace communication
{

    void SocketServer::bind()
    {
        const auto addr = this->create_ip_address();

        const int error = winsock2_bind(this->sock, (sockaddr*)&addr, static_cast<int>(sizeof(addr)));
        if (error == SOCKET_ERROR) {
            std::cerr << "can't bind. error: " + std::to_string(WSAGetLastError()) << std::endl;
            throw Error::Bind;
        }
    }

    void SocketServer::listen()
    {
        const auto listen_sock = this->sock;
        // TODO 第二引数のbacklogの意味調べる
        const int err = winsock2_litsen(listen_sock, 1);
        if (err == SOCKET_ERROR) {
            std::cerr << "can't listen. error: " + std::to_string(WSAGetLastError()) << std::endl;
            throw Error::Listen;
        }
    }

    void SocketServer::accept()
    {
        const auto listen_sock = this->sock;

        std::cerr << "Waiting for client to connect..." << std::endl;
        const SOCKET accept_sock = winsock2_accept(listen_sock, NULL, NULL);
        if (accept_sock == INVALID_SOCKET) {
            std::cerr << "can't accpet. error: " + std::to_string(WSAGetLastError()) << std::endl;
            throw Error::Accpet;
        }
        else {
            std::cerr << "Client connected." << std::endl;
            closesocket(listen_sock);
            this->sock = accept_sock;
        }
    }
 
} // namespace communication
