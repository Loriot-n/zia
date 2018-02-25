#pragma once

#include <vector>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


namespace zia {
	class Stream {
		public:
			~Stream();
			Stream();

			int getSock() const { return _socket; }
			void setSock(int socket) { _socket = socket; }

			struct sockaddr_in getAddr() const { return _addr; }
			void setAddr(struct sockaddr_in &addr) { _addr = addr; }
			bool isTLS() const { return _isTLS; } 
			void setTLS(bool isTLS) { _isTLS = isTLS; }

			int recv(void* buf, size_t len, unsigned int timeout) {
				struct timeval tv;
				tv.tv_sec = timeout;
				tv.tv_usec = 0;
				::setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));
				return ::recv(_socket, buf, len, 0);
			};
			int send(const void* buf, size_t len) { return ::send(_socket, buf, len, 0); }

		private:
			bool _isTLS;
			int _socket;
			struct sockaddr_in _addr;
	};

}
