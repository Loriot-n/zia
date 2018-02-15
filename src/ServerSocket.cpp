#include "ServerSocket.hpp"

namespace zia {

	ServerSocket::ServerSocket(api::Conf &config) {

		// Check for config value
		auto searchPort = config.find("port");
		if (searchPort != config.end())
			_port = std::get<long long>(searchPort->second.v);
		else
			_port = 80;

		auto searchSizeMax = config.find("sizeMax");
		if (searchSizeMax != config.end())
			_sizeMax = std::get<long long>(searchSizeMax->second.v);
		else
			_sizeMax = 2 * /* GB */1000000000;

		if (!Socket::create(_sizeMax))
			throw SocketException("Could not create Socket.");

		if (!Socket::bind(_port))
			throw SocketException("Could not bind socket to port.");

		if (!Socket::listen())
			throw SocketException("Could not listen socket.");
		
		if (!Socket::epoll_create())
			throw SocketException("Could not create epoll_fd.");

		if (Socket::epoll_ctl(*this) < 0)
			throw SocketException("Could not set epoll on main server socket.");
	}

	const Socket& ServerSocket::operator << (const api::Net::Raw &s) const {
		if (!Socket::send(s))
			throw SocketException("Could not write on Socket.");
		return *this;
	}

	const Socket& ServerSocket::operator >> (api::Net::Raw &s) const {
		int res = 0;
		if ((res = Socket::recv(s)) < 0)
			throw SocketException("Could not read from Socket.");
		else if (res == 0)
			throw SocketException("Client closed the connection.");
		return *this;
	}

	void ServerSocket::accept(ServerSocket &s) {
		if (!Socket::accept(s))
			throw SocketException("Could not accept Socket.");

		if (Socket::epoll_ctl(s))
			throw SocketException("Could not set epoll on new client.");
	}

}