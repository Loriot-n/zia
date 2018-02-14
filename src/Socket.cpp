#include "Socket.hpp"

namespace zia {

	Socket::Socket() : _sock(-1) {

		#ifdef _WIN32
		if (winStartup() != 0)
			throw Exception("winStartup");
		#endif
		memset(&_addr, 0, sizeof(_addr));
	}

	Socket::~Socket() {
		if (isValid())
			::close(_sock);
	}

	bool Socket::create() {
		_sock = socket(AF_INET, SOCK_STREAM, 0);
		if (!isValid())
			return false;

		int on = 1;
		if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, (const char *) &on, sizeof(on)) == -1)
			return false;
		return true;
	}

	bool Socket::bind(const int port) {
		if (!isValid())
			return false;

		_addr.sin_family = AF_INET;
		_addr.sin_addr.s_addr = INADDR_ANY;
		_addr.sin_port = htons(port);

		if (::bind(_sock, (struct sockaddr *) &_addr, sizeof(_addr)) == -1)
			return false;
		return true;
	}

	bool Socket::listen() const {
		if (!isValid())
			return false;
		// NOTE: Is 10 Enough?
		if (::listen(_sock, 10) == -1)
			return false;
		return true;
	}

	bool Socket::accept(Socket &s) const {

		int addr_len = sizeof(_addr);
		s._sock = ::accept(_sock, (sockaddr *) &_addr, (socklen_t *) &addr_len);

		return (s._sock <= 0 ? false : true);
	}

	bool Socket::isValid() const {
		return (_sock != -1);
	}

	bool Socket::send(const api::Net::Raw &raw) const {
		
		const char *s = (const char *) raw.data();

		int ret = ::send(_sock, s, raw.size(), MSG_NOSIGNAL);
		return (ret == -1 ? false : true);
	}

	int Socket::recv(api::Net::Raw &) const {

	}

#ifdef _WIN32
	int Socket::winStartup() {

		WSADATA wsaData;

		return (!WSAStartup(MAKEWORD(2, 2), &wsaData));
	}
#endif


}