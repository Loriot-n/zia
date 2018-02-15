#pragma once

#include "Socket.hpp"
#include "api/Conf.hpp"

namespace zia {

	class ServerSocket : private Socket {

		public:
			ServerSocket(api::Conf &);
			ServerSocket() {};
			~ServerSocket() {};

			const Socket& operator << (const api::Net::Raw &) const;
			const Socket& operator >> (api::Net::Raw &) const;

			void accept(ServerSocket &);

		protected:
			long long _port;
			long long _sizeMax;
	};
}