#pragma once

#include <iostream>

#include "ServerException.hpp"

namespace zia {

    class TransferException : protected ServerException {

        public:
            TransferException(const char* message): ServerException(message) { _context = "TransferException : "; }
            TransferException(const std::string& message): ServerException(message) { _context = "TransferException : "; }
            virtual ~TransferException() throw () {}

            virtual const char* what() const throw () {
                std::string msg;
                msg = _context + _msg;
               return msg.c_str();
            }

        protected:
            std::string _msg;

        private:
            std::string _context;
    };
}