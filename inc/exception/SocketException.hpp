#pragma once

#include <iostream>

#include "ServerException.hpp"

namespace zia {

    class SocketException : public ServerException {

        public:
            SocketException(const char* message)
	      : ServerException(std::string("SocketException : ") + message) {}
            SocketException(const std::string& message)
	      : ServerException("SocketException : " + message) {}
            virtual ~SocketException() throw () {}
    };
}
