#pragma once

#include "socket_tcp.h"

namespace communication
{

    class SocketClient : public SocketTCP
    {
    public:
        using SocketTCP::SocketTCP;

        void connect();
    };

} // namespace communication
