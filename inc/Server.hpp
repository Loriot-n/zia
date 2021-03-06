#pragma once

#include <iostream>
#include <thread>
#include <functional>
#include <vector>
#include <mutex>

#include "api/Net.hpp"
#include "api/Conf.hpp"
#include "api/Http.hpp"
#include "Config.hpp"


#include "socket/Reactor.hpp"
#include "socket/Listener.hpp"
#include "socket/Stream.hpp"

namespace zia {

	class Server : public api::Net {


		public:

			Server(Config const &c);
			~Server();

			bool run(Callback) override;
			bool send(api::ImplSocket *, const api::Net::Raw &) override;
			bool stop() override;

			void updateConfig(Config const &);
			//static void handleRequest(Raw, api::NetInfo);

		protected:
			long long _port;
			long long _sslPort;
			long long _timeout;
			long long _maxEv;
			long long _threadPoolSize;
			std::mutex _mutex;
			api::HttpDuplex _duplex;

	};
}
