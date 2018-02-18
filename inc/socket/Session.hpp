#pragma once

#include <memory>
#include <iostream>

#include "Handler.hpp"
#include "Stream.hpp"


namespace zia {

    class Session : public Handler {

        public:
            ~Session();
            Session();

            Stream& stream() { return _stream; }

            int getHandler() const { return _stream.getSock(); }
            void handleReq(api::Net::Raw, api::NetInfo);
            int recv(api::Net::Raw &req) const;

        private:
            Stream _stream;
            unsigned int _timeout;
    };

    typedef std::shared_ptr<Session> SessionPtr;
}