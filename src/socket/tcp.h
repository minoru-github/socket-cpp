#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib,"Ws2_32.lib")

class tcp
{
    const int initialize_winsock();
    void finalize_socket_communication(
        const SOCKET sock
    );
    const sockaddr_in create_ip_address();
    const SOCKET create_socket();
    const int bind(
        const SOCKET listen_sock
    );
    const int listen(
        const SOCKET listen_sock
    );
    const SOCKET accept(
        const SOCKET listen_sock
    );
    const int connect_to_server(
        const SOCKET sock
    );
    void server();
    void client();

    const int send_msg(
        const SOCKET sock,
        const std::string msg
    );
    const std::string receive_msg(
        const SOCKET sock
    );
};

