#pragma once

#include <iostream>

namespace zia {

	class ServerException : public std::runtime_error {

		public:

            ServerException(const char* message)
	      : runtime_error(message) {}
            ServerException(const std::string& message)
	      : runtime_error(message) {}

            virtual ~ServerException() throw () = default;
	};
}
