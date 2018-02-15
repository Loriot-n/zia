#pragma once

#include <iostream>

#ifdef _WIN32
#	include <sys/epoll.h>
#endif

#include <api/Net.hpp>
#include <api/Conf.hpp>

#include "ServerSocket.hpp"

namespace zia {

	struct ImplSocket {
		Socket s;
	};

	class RequestHandler : public api::Net {

		public:
			RequestHandler(api::Conf &);
			~RequestHandler();

			bool run(Callback cb);
			bool send(api::ImplSocket *, const api::Net::Raw &resp);
			bool stop();

		private:
			ServerSocket _serverSocket;
			int 		 _epollFd;
			unsigned int _threadCount;
	};
}