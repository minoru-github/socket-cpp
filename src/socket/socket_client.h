#pragma once

#include "tcp.h"

namespace communication {

    class SocketClient : public SocketTCP {
    public:
        SocketClient();
        ~SocketClient();

        void connect();
    };

} // namespace communication
