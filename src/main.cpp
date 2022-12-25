#include "socket/tcp.h"

void run_server();
void run_client();

using namespace communication;

int main()
{
    //run_server();
    run_client();

    return 0;
}

void run_client() {
    auto client = Client();
    client.socket();
    client.connect();
    client.send("hello world\n");
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
}