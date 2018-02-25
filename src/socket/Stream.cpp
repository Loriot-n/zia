#include "socket/Stream.hpp"


namespace zia {

	Stream::Stream() : _socket(0) {}

	Stream::~Stream() { ::close(_socket); }

	int Stream::recv(void* buf, size_t len, unsigned int timeout)
	{
		struct timeval tv;

		tv.tv_sec = timeout;
		tv.tv_usec = 0;

		::setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));

		return ::recv(_socket, buf, len, 0);
	}

	int Stream::send(const void* buf, size_t len)
	{
		return ::send(_socket, buf, len, 0);
	}
}