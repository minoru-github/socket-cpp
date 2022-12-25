#pragma once

#include "socket_tcp.h"

namespace communication
{

    class SocketServer : public SocketTCP
    {
    public:
        using SocketTCP::SocketTCP;

        void bind();
        void listen();
        void accept();
    };

} // namespace communication
