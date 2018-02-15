#include "RequestHandler.hpp"

namespace zia {

	RequestHandler::RequestHandler(api::Conf &conf) {

	}

	RequestHandler::~RequestHandler() {}

	bool RequestHandler::run(Callback cb) {

	}

	bool RequestHandler::send(api::ImplSocket *, const api::Net::Raw &resp) {

	}

	bool RequestHandler::stop() {

	}

	void RequestHandler::waitLoop() {

		std::array<struct epoll_event, 32> events;

		while (true) {
			int nfds = ::epoll_wait(_epollFd, events.data(), 32, -1);
			if (nfds < 0)
				throw SocketException("Error while waiting socket.");

			for (int i = 0 ; i < nfds ; ++i) {

				if (events[i].data.fd == _serverSocket.getSockFd()) {
					// Accept new client
					ServerSocket newSock;
					_serverSocket.accept(newSock);
				} else {
					// Handle request
				}

			}
		}
	}

}