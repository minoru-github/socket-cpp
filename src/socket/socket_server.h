#pragma once

#include "tcp.h"

namespace communication {

    class SocketServer : public SocketTCP {
    public:
        SocketServer();
        ~SocketServer();

        void bind();
        void listen();
        void accept();
    };

} // namespace communication
