#pragma once

#include <iostream>

namespace zia {

    class SocketException : public std::exception
    {
        public:

            explicit SocketException(const char* message): msg_(message) {}

            explicit SocketException(const std::string& message): msg_(message) {}

            virtual ~SocketException() throw () {}

            virtual const char* what() const throw () {
               return msg_.c_str();
            }

        protected:
            std::string msg_;
    };
}