#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib,"Ws2_32.lib")

const int initialize();
const sockaddr_in create_ip_address();
const int client();

int main()
{
    const int err_initialize = initialize();
    const int err_client = client();

    return 0;
}

const int initialize() {
    WSADATA data{};
    const auto version_requested = MAKEWORD(2, 0);
    // WSAStartupでWinsockの初期化を行う。
    // Winsockアプリの最初にWSAStartupは行う必要がある。
    // https://learn.microsoft.com/ja-jp/windows/win32/winsock/initializing-winsock
    const int err_startup = WSAStartup(version_requested, &data);
    if (err_startup) {
        std::cerr << "can't startup. error: " + std::to_string(err_startup);
    }
    return err_startup;
}

const std::string server_ip = "127.0.0.1";
constexpr int port = 8080;
constexpr int address_family = AF_INET;
constexpr int type = SOCK_STREAM;
constexpr int protocol = IPPROTO_HOPOPTS;
const sockaddr_in create_ip_address()
{
    sockaddr_in addr{};
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;

    // IPアドレスをバイナリ形式に変換
    inet_pton(addr.sin_family, server_ip.c_str(), &addr.sin_addr);

    return addr;
}

const int client() {
    const auto addr = create_ip_address();

    const SOCKET sock = socket(address_family, type, protocol);
    if (sock == INVALID_SOCKET) {
        std::cerr << "can't create socket.";
        return 1;
    }

    const auto namelen = sizeof(addr);
    const int err_connect = connect(sock, (sockaddr*)&addr, static_cast<int>(namelen));
    if (err_connect) {
        std::cerr << "can't connect. error: " + std::to_string(err_connect);
        return err_connect;
    }

    const std::string msg = "hello world\n";
    const int err_send = send(sock, msg.c_str(), static_cast<int>(msg.length()), 0);
    if (err_send == SOCKET_ERROR) {
        std::cerr << "can't send message. error: " + std::to_string(err_send);
        return err_send;
    }

    closesocket(sock);
    WSACleanup();
}