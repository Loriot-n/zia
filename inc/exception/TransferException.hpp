#pragma once

#include <iostream>

#include "ServerException.hpp"

namespace zia {

    class TransferException : public ServerException {

        public:
            TransferException(const char* message)
	      : ServerException(std::string("TransferException : ") + message) {}
            TransferException(const std::string& message)
	      : ServerException("TransferException : " + message) {}
            virtual ~TransferException() throw () {}
    };
}
