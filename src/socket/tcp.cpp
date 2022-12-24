#include "tcp.h"

const int initialize_winsock() {
    WSADATA data{};
    const auto version_requested = MAKEWORD(2, 2);
    // WSAStartupでWinsockの初期化を行う。
    // Winsockアプリの最初にWSAStartupは行う必要がある。
    // https://learn.microsoft.com/ja-jp/windows/win32/winsock/initializing-winsock
    const int err_startup = WSAStartup(version_requested, &data);
    if (err_startup) {
        std::cerr << "can't startup. error: " + std::to_string(err_startup) << std::endl;
    }
    return err_startup;
}

void finalize_socket_communication(const SOCKET sock) {
    closesocket(sock);
    WSACleanup();
}

const sockaddr_in create_ip_address()
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

const int bind(
    const SOCKET listen_sock
) {
    const auto addr = create_ip_address();

    const int error = bind(listen_sock, (sockaddr*)&addr, static_cast<int>(sizeof(addr)));
    if (error == SOCKET_ERROR) {
        std::cerr << "can't bind. error: " + std::to_string(WSAGetLastError()) << std::endl;
        finalize_socket_communication(listen_sock);
    }
    return error;
}

const SOCKET create_socket() {
    constexpr int address_family = AF_INET;
    constexpr int type = SOCK_STREAM;
    constexpr int protocol = IPPROTO_TCP;

    const SOCKET sock = socket(address_family, type, protocol);
    if (sock == INVALID_SOCKET) {
        std::cerr << "can't create socket. error: " + std::to_string(WSAGetLastError()) << std::endl;
        WSACleanup();
    }
    return sock;
}

const int listen(
    const SOCKET listen_sock
) {
    // TODO 第二引数のbacklogの意味調べる
    const int err = listen(listen_sock, 1);
    if (err == SOCKET_ERROR) {
        std::cerr << "can't listen. error: " + std::to_string(WSAGetLastError()) << std::endl;
        finalize_socket_communication(listen_sock);
    }

    return err;
}

const SOCKET accept(
    const SOCKET listen_sock
) {
    std::cerr << "Waiting for client to connect..." << std::endl;
    const SOCKET accpet_socket = accept(listen_sock, NULL, NULL);
    if (accpet_socket == INVALID_SOCKET) {
        std::cerr << "can't accpet. error: " + std::to_string(WSAGetLastError()) << std::endl;
        finalize_socket_communication(listen_sock);
    }
    else {
        std::cerr << "Client connected." << std::endl;
        closesocket(listen_sock);
    }
    return accpet_socket;
}

const int connect_to_server(
    const SOCKET sock
) {
    const auto addr = create_ip_address();

    const int error = connect(sock, (sockaddr*)&addr, static_cast<int>(sizeof(addr)));
    if (error) {
        std::cerr << "can't connect. error: " + std::to_string(WSAGetLastError()) << std::endl;
        finalize_socket_communication(sock);
    }
    return error;
}

const int send_msg(
    const SOCKET sock,
    const std::string msg
) {
    const int res_send = send(sock, msg.c_str(), static_cast<int>(msg.length()), 0);
    // res_sendは正常時は送信したバイト数
    if (res_send == SOCKET_ERROR) {
        std::cerr << "can't send message. error: " + std::to_string(WSAGetLastError()) << std::endl;
        finalize_socket_communication(sock);
    }
    return res_send;
}

const std::string receive_msg(
    const SOCKET sock
) {
    // TODO 汎用性高める
    constexpr size_t default_buflen = 512;
    char recvbuf[default_buflen] = { 0 };
    int recvbuflen = default_buflen;

    const int received_size = recv(sock, recvbuf, recvbuflen, 0);

    if (received_size == 0) {
        finalize_socket_communication(sock);
    }
    else if (received_size < 0) {
        std::cerr << "can't receive message. error: " + std::to_string(WSAGetLastError()) << std::endl;
        finalize_socket_communication(sock);
    }

    const std::string msg = recvbuf;
    return msg;
}

void client() {
    const int err_initialize = initialize_winsock();
    if (err_initialize) {
        return;
    }

    const SOCKET sock = create_socket();
    if (sock == INVALID_SOCKET) {
        return;
    }

    const int err_connect = connect_to_server(sock);
    if (err_connect) {
        return;
    }

    const int err_send = send_msg(sock, "hello world\n");
    if (err_send == SOCKET_ERROR) {
        return;
    }

    finalize_socket_communication(sock);

    return;
}

void server() {
    const int err_initialize = initialize_winsock();

    // listener
    const SOCKET listen_sock = create_socket();
    if (listen_sock == INVALID_SOCKET) {
        return;
    }

    const int err_bind = bind(listen_sock);
    if (err_bind == SOCKET_ERROR) {
        return;
    }

    const int err_listen = listen(listen_sock);
    if (err_listen == SOCKET_ERROR) {
        return;
    }

    const SOCKET sock = accept(listen_sock);
    if (sock == INVALID_SOCKET) {
        return;
    }

    constexpr size_t default_buflen = 512;
    char recvbuf[default_buflen] = { 0 };
    int recvbuflen = default_buflen;
    while (1) {
        const std::string msg = receive_msg(sock);
        if (msg.length() > 0) {
            std::cerr << "received msg: " + msg << std::endl;
            const int err_send = send_msg(sock, "Thanks for your msg.\n");
            if (err_send == SOCKET_ERROR) {
                return;
            }
        }
        else {
            break;
        }
    }

    return;
}