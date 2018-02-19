#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>

#include "socket/Handler.hpp"
#include "socket/Reactor.hpp"
#include "api/Conf.hpp"

namespace zia {


	class Listener;
	typedef std::shared_ptr<Listener> ListenerPtr;

	class Stream;
	class Listener : public Handler {

		private:
			Listener(ReactorPtr server, ReactorPtr session, unsigned int timeout, unsigned int port);

		public:
			Listener(const Listener &) = delete;
			~Listener();
			Listener& operator=( const Listener & ) = delete;

			static ListenerPtr create(
				ReactorPtr listenerReactor,
				ReactorPtr connectionReactor,
				unsigned int timeout,
				unsigned int port) {
					return ListenerPtr(new Listener(listenerReactor, connectionReactor, timeout, port));
			}

			void accept(Stream &);
			void open();
			int getHandler() const { return _socket; }
			SockState handleInput(api::Net::Callback);
			void acceptNewClient();



		protected:
			int _socket;

			ReactorPtr _server;
			ReactorPtr _session;

			unsigned int _timeout;
			unsigned int _port;
	};
}