#include <iostream>
#include <memory>

#include "socket/socket_server.h"
#include "socket/socket_client.h"

void run_server();
void run_client();

using namespace communication;

int main()
{
    //run_server();
    run_client();

    return 0;
}

void run_client()
{
    try {
        std::unique_ptr<SocketClient> client(new SocketClient());
        client->socket();
        client->connect();
        client->send("hello world\n");
    }
    catch (SocketTCP::Error error) {
        std::cerr << "error: " + std::to_string(static_cast<unsigned int>(error)) << std::endl;
    }
}

void run_server()
{
    try {
        std::unique_ptr<SocketServer> server(new SocketServer());
        server->socket();
        server->bind();
        server->listen();
        server->accept();

        while (1) {
            const std::string msg = server->receive();
            if (msg.length() > 0) {
                std::cerr << "received msg: " + msg << std::endl;
                server->send("Thanks for your msg.\n");
            }
            else {
                break;
            }
        }
    }
    catch (SocketTCP::Error error) {
        std::cerr << "error: " + std::to_string(static_cast<unsigned int>(error)) << std::endl;
    }
}