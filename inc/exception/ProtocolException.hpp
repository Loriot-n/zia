#pragma once

#include <iostream>

#include "ServerException.hpp"

namespace zia {

    class ProtocolException : public ServerException {

        public:
            ProtocolException(const char* message)
	      : ServerException(std::string("ProtocolException : ") + message) {}
            ProtocolException(const std::string& message)
	      : ServerException("ProtocolException : " + message) {}
            virtual ~ProtocolException() throw () {}
    };
}
