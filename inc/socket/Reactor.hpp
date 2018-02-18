#pragma once

#include <mutex>
#include <map>
#include <unistd.h>
#include <string.h>

#include "api/Net.hpp"
#include "Handler.hpp"
#include "SocketException.hpp"
#ifndef _WIN32
# include <sys/epoll.h>
#endif

namespace zia {

	class Reactor {

		public:
			Reactor(int maxEv);
			~Reactor();

			void registerSock(HandlerPtr);
			void resumeSock(HandlerPtr);
			void deleteSock(HandlerPtr);
			void handleEvents(api::Net::Callback);


		private:
			api::Net::Callback cb;
			int _epfd;
			struct epoll_event _epev;
			std::map<int, HandlerPtr> _handlers;
			std::mutex _mutex;

	};

	typedef std::shared_ptr<Reactor> ReactorPtr;
}