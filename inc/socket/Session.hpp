#pragma once

#include <memory>
#include <iostream>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>

#include "Handler.hpp"
#include "Stream.hpp"
#include "Server.hpp"
#include "http/HttpParser.hpp"

#include "exception/TransferException.hpp"

namespace zia {

    struct ImplSocket : api::ImplSocket {
        Stream stream;
    };

    class Session : public Handler {

        public:
            ~Session();
            Session(unsigned int timeout);

            Stream& stream() { return _stream; }

            int getHandler() const { return _stream.getSock(); }
            SockState handleInput(api::Net::Callback);
            int getClientInfo(api::NetInfo &netInfo);
            unsigned int recv(api::Net::Raw &req);

        private:
            Stream _stream;
            unsigned int _timeout;
            zia::api::ImplSocket _implSocket;
    };

    typedef std::shared_ptr<Session> SessionPtr;
}