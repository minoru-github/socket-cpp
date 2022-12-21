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

    const auto server_ip = "127.0.0.1";
    constexpr int address_family = AF_INET;
    constexpr int type = SOCK_STREAM;
    constexpr int protocol = IPPROTO_HOPOPTS;
    sockaddr_in addr{};
    addr.sin_port = htons(8080);
    addr.sin_family = AF_INET;

    // IPアドレスをバイナリ形式に変換
    inet_pton(addr.sin_family, server_ip, &addr.sin_addr);

    const SOCKET sock = socket(address_family, type, protocol);
    if (sock == INVALID_SOCKET) {
        std::cerr << "can't create socket.";
        return 1;
    }

    const auto namelen = sizeof(addr);
    const auto err_connect = connect(sock, (sockaddr*)&addr, static_cast<int>(namelen));
    if (err_connect) {
        std::cerr << "can't connect. error: " + std::to_string(err_connect);
        return err_connect;
    }

    const std::string msg("hello world\n");
    const auto err_send = send(sock, msg.c_str(), msg.length(), 0);
    if (err_send == SOCKET_ERROR) {
        std::cerr << "can't send message. error: " + std::to_string(err_send);
        return err_send;
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}
