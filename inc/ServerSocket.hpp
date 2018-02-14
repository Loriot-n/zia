#pragma once

#include "Socket.hpp"

namespace zia {

	class ServerSocket : private Socket {

		const Socket& operator << (const std::string &) const;
		const Socket& operator >> (std::string &) const;

		void accept(ServerSocket &);

	};
}