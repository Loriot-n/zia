#pragma once

#include <iostream>

namespace zia {

	class ServerException : public std::exception {
		
		public:
            
            ServerException(const char* message): _msg(message) { _context = "ServerException : "; }
            ServerException(const std::string& message): _msg(message) { _context = "ServerException : "; }
            virtual ~ServerException() throw () {}

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