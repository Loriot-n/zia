#include "socket/Session.hpp"

namespace zia {

	Session::Session(unsigned int timeout): _timeout(timeout) {

	}

	Session::~Session() {

	}

	int Session::getClientInfo(api::NetInfo &netInfo) {

		netInfo.time = std::chrono::system_clock::now();
		netInfo.start = std::chrono::steady_clock::now();

		struct sockaddr_in clientaddr = _stream.getAddr();

		//Get IP in readable form
		char buf[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, (struct sockaddr *)&clientaddr, buf, sizeof(clientaddr));

		netInfo.ip.i = clientaddr.sin_addr.s_addr;
		netInfo.ip.str = std::string(buf);
		netInfo.port = clientaddr.sin_port;

		_implSocket.stream = &_stream;
		_implSocket.isTLS = _stream.isTLS();
		netInfo.sock = &_implSocket;
		return (0);
	}

	SockState Session::handleInput(api::Net::Callback cb) {

		api::Net::Raw r;
		api::NetInfo  netinfo;

		try {
			getClientInfo(netinfo);

		} catch (TransferException &e) {

			// FIXME: Doesnt catch exception
			std::cout << e.what() << std::endl;
			return SockState::CLOSE;
		} catch (const std::exception &e) {
			std::cerr << e.what() << std::endl;
			return SockState::CLOSE;
		}
		cb(r, netinfo);
		return SockState::CLOSE;
	}
}