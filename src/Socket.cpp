#include "Socket.hpp"
#include <stdio.h>

namespace zia {

	Socket::Socket() : _sock(-1) {

		#ifdef _WIN32
		if (winStartup() != 0)
			throw Exception("winStartup");
		#endif
		memset(&_addr, 0, sizeof(_addr));
	}

	Socket::Socket(uint64_t sizeMax) : _sock(-1), _sizeMax(sizeMax) {

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

	bool Socket::create(uint64_t sizeMax) {
		_sock = ::socket(AF_INET, SOCK_STREAM, 0);
		_sizeMax = sizeMax;
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
		
		int ret = ::send(_sock, reinterpret_cast<const char *>(raw.data()), raw.size(), MSG_NOSIGNAL);
		return (ret == -1 ? false : true);
	}

	int Socket::recv(api::Net::Raw &req) const {

		long pageSize = ::sysconf(_SC_PAGESIZE);
		unsigned char buf[pageSize];
		bzero(buf, pageSize);
		if (req.empty())
			req.reserve(pageSize);

		int r = 0;
		uint64_t totalRead = 0; 
		while ((r = ::read(_sock, buf, pageSize - 1)) != 0) {

			if (r == -1) {
				std::cerr << "Error while reading sock #" << _sock << std::endl;
				return (-1);
			}

			if (req.size() > _sizeMax)
				return (-1);

			// If we read more than the end of the container, re-allocate enough size
			if (req.capacity() < totalRead && r > 0) {

				// If the data is too big for max_capacity of vector size : too bad. Shouldn't have used f*ckin vectors for buffers.
				try {
					req.reserve(totalRead + pageSize);
				} catch (const std::length_error &e) {
					std::cerr << e.what() << std::endl;
					return (-1);
				}
			}

			req.insert(req.end(), (std::byte *)&buf[0], (std::byte *)&buf[r - 1]);

			totalRead += r;
		}
		return totalRead;
	}

#ifdef _WIN32
	int Socket::winStartup() {

		WSADATA wsaData;

		return (!WSAStartup(MAKEWORD(2, 2), &wsaData));
	}
#endif


}