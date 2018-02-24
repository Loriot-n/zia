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

		_implSocket.stream = _stream;
		_implSocket.isTLS = _stream.isTLS();
		netInfo.sock = &_implSocket;
		return (0);
	}

	unsigned int Session::recv(api::Net::Raw &req) {

		long pageSize = ::sysconf(_SC_PAGESIZE);
		char buf[pageSize];
		::bzero(buf, pageSize);
		if (req.empty())
			req.reserve(pageSize);

		uint64_t totalRead = 0;

		// THIS IS TEMPORARY
		// Check for a \r\n\r\n in vector to check the end of headers	
		while (1) {

			int r = _stream.recv(buf, pageSize, _timeout);

			// std::string header(buf, s);
			// if header.find("Content-Length")

			if (r <= 0) {
				return totalRead;
			}
			req.insert(req.end(), (std::byte *)&buf[0], (std::byte *)&buf[r - 1]);
			::bzero(buf, pageSize);
			totalRead += r;
		}
		return totalRead;
	}

	SockState Session::handleInput(api::Net::Callback cb) {

		api::Net::Raw r;
		api::NetInfo  netinfo;

		try {

			getClientInfo(netinfo);
			this->recv(r);

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