#pragma once

#include <winsock2.h>
#include <string>

#pragma comment(lib,"Ws2_32.lib")

namespace communication
{
    constexpr size_t recvbuflen = 512;

    class SocketTCP
    {
    protected:
        SocketTCP();
        ~SocketTCP();
        SOCKET sock = INVALID_SOCKET;
        char recvbuf[recvbuflen] = { 0 };
        const sockaddr_in create_ip_address();

    public:
        void socket();
        const std::string receive();
        void send(const std::string msg);

        enum class Error {
            Construct,
            Socket,
            IpAddress,
            Bind,
            Listen,
            Accpet,
            Connect,
            Send,
            Receive,
            Deconstruct,
        };
    };

} // namespace communication
