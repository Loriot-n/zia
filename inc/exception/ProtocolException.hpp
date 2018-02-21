#pragma once

#include <iostream>

#include "ServerException.hpp"

namespace zia {

    class ProtocolException : protected ServerException {

        public:
            ProtocolException(const char* message): ServerException(message) { _context = "ProtocolException : "; }
            ProtocolException(const std::string& message): ServerException(message) { _context = "ProtocolException : "; }
            virtual ~ProtocolException() throw () {}

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