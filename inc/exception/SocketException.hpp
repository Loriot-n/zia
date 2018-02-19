#pragma once

#include <iostream>

#include "ServerException.hpp"

namespace zia {

    class SocketException : protected ServerException {

        public:
            SocketException(const char* message): ServerException(message) { _context = "SocketException : "; }
            SocketException(const std::string& message): ServerException(message) { _context = "SocketException : "; }
            virtual ~SocketException() throw () {}

            virtual const char* what() const throw () {
               return (_context + _msg).c_str();
            }

        protected:
            std::string _msg;

        private:
            std::string _context;
    };
}