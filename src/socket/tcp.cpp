#include "tcp.h"
#include <ws2tcpip.h>

auto winsock2_socket = socket;
auto winsock2_bind = bind;
auto winsock2_litsen = listen;
auto winsock2_accept = accept;
auto winsock2_receive = recv;
auto winsock2_send = send;
auto winsock2_connect = connect;

namespace communication
{

    SocketTCP::SocketTCP()
    {
        WSADATA data{};
        const auto version_requested = MAKEWORD(2, 2);
        // WSAStartupでWinsockの初期化を行う。
        // Winsockアプリの最初にWSAStartupは行う必要がある。
        // https://learn.microsoft.com/ja-jp/windows/win32/winsock/initializing-winsock
        const int err_startup = WSAStartup(version_requested, &data);
        if (err_startup) {
            std::cerr << "can't startup. error: " + std::to_string(err_startup) << std::endl;
        }
    };
    SocketTCP::~SocketTCP()
    {
        closesocket(this->sock);
        WSACleanup();
    };

    void SocketTCP::socket()
    {
        constexpr int address_family = AF_INET;
        constexpr int type = SOCK_STREAM;
        constexpr int protocol = IPPROTO_TCP;

        const SOCKET sock = winsock2_socket(address_family, type, protocol);

        if (sock == INVALID_SOCKET) {
            std::cerr << "can't create socket. error: " + std::to_string(WSAGetLastError()) << std::endl;
            WSACleanup();
        }

        this->sock = sock;
    };

    const sockaddr_in SocketTCP::create_ip_address()
    {
        const std::string server_ip = "127.0.0.1";
        constexpr int port = 8080;

        sockaddr_in addr{};
        addr.sin_port = htons(port);
        addr.sin_family = AF_INET;

        // IPアドレスをバイナリ形式に変換
        inet_pton(addr.sin_family, server_ip.c_str(), &addr.sin_addr);

        return addr;
    }

    const std::string SocketTCP::receive()
    {
        // TODO 汎用性高める
        const int received_size = winsock2_receive(this->sock, this->recvbuf, recvbuflen, 0);

        if (received_size == 0) {
            //finalize_socket_communication(sock);
        }
        else if (received_size < 0) {
            std::cerr << "can't receive message. error: " + std::to_string(WSAGetLastError()) << std::endl;
            //finalize_socket_communication(sock);
        }

        const std::string msg = recvbuf;
        return msg;
    }

    void SocketTCP::send(const std::string msg)
    {
        const int res_send = winsock2_send(sock, msg.c_str(), static_cast<int>(msg.length()), 0);
        // res_sendは正常時は送信したバイト数
        if (res_send == SOCKET_ERROR) {
            std::cerr << "can't send message. error: " + std::to_string(WSAGetLastError()) << std::endl;
            //finalize_socket_communication(sock);
        }
    }

    Server::Server()
    {
    }

    Server::~Server()
    {
    }

    void Server::bind()
    {
        const auto addr = this->create_ip_address();

        const int error = winsock2_bind(this->sock, (sockaddr*)&addr, static_cast<int>(sizeof(addr)));
        if (error == SOCKET_ERROR) {
            std::cerr << "can't bind. error: " + std::to_string(WSAGetLastError()) << std::endl;
            //finalize_socket_communication(listen_sock);
        }
    }

    void Server::listen()
    {
        const auto listen_sock = this->sock;
        // TODO 第二引数のbacklogの意味調べる
        const int err = winsock2_litsen(listen_sock, 1);
        if (err == SOCKET_ERROR) {
            std::cerr << "can't listen. error: " + std::to_string(WSAGetLastError()) << std::endl;
            //finalize_socket_communication(listen_sock);
        }
    }

    void Server::accept()
    {
        const auto listen_sock = this->sock;

        std::cerr << "Waiting for client to connect..." << std::endl;
        const SOCKET accept_sock = winsock2_accept(listen_sock, NULL, NULL);
        if (accept_sock == INVALID_SOCKET) {
            std::cerr << "can't accpet. error: " + std::to_string(WSAGetLastError()) << std::endl;
            //finalize_socket_communication(listen_sock);
        }
        else {
            std::cerr << "Client connected." << std::endl;
            closesocket(listen_sock);
            this->sock = accept_sock;
        }
    }

    Client::Client()
    {
    }

    Client::~Client()
    {
    }

    void Client::connect()
    {
        const auto addr = create_ip_address();

        const int error = winsock2_connect(this->sock, (sockaddr*)&addr, static_cast<int>(sizeof(addr)));
        if (error) {
            std::cerr << "can't connect. error: " + std::to_string(WSAGetLastError()) << std::endl;
            //finalize_socket_communication(sock);
        }
    }
} // namespace communication
