#include "socket/tcp.h"

void run_server();
void client();

using namespace communication;

int main()
{
    run_server();
   // client();

    return 0;
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

void run_server() {
    auto server = Server();
    server.socket();
    server.bind();
    server.listen();
    server.accept();

    while (1) {
        const std::string msg = server.receive();
        if (msg.length() > 0) {
            std::cerr << "received msg: " + msg << std::endl;
            server.send("Thanks for your msg.\n");
        }
        else {
            break;
        }
    }

    return;
}