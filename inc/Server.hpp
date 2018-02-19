#pragma once

#include <iostream>
#include <thread>
#include <functional>
#include <vector>

#include "api/Net.hpp"
#include "api/Conf.hpp"

#include "socket/Reactor.hpp"
#include "socket/Listener.hpp"

namespace zia {

	class Server : public api::Net {

		public:

			Server(api::Conf &c);
			~Server();

			bool run(Callback cb);
			bool send(api::ImplSocket *, const api::Net::Raw &);
			bool stop();
			static void handleRequest(Raw, api::NetInfo);

		protected:
			long long _port;
			long long _timeout;
			long long _maxEv;
			long long _threadPoolSize;
			Callback  _cb;

	};
}