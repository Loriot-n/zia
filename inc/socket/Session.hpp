#pragma once

#include <memory>
#include <iostream>
#include <strings.h>
#include <string.h>
#include <sys/socket.h>

#include "Handler.hpp"
#include "Stream.hpp"

#include "exception/TransferException.hpp"

namespace zia {

    class Session : public Handler {

        public:
            ~Session();
            Session();

            Stream& stream() { return _stream; }

            int getHandler() const { return _stream.getSock(); }
            SockState handleInput(api::Net::Callback);
            int recv(api::Net::Raw &req);

        private:
            Stream _stream;
            unsigned int _timeout;
    };

    typedef std::shared_ptr<Session> SessionPtr;
}