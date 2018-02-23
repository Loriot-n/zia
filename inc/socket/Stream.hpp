#pragma once

#include <vector>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


namespace zia {
	class Stream
	{
		public:
			~Stream();
			Stream();

			int getSock() const { return _socket; }
			void setSock(int socket) { _socket = socket; }

			struct sockaddr_in getAddr() const { return _addr; }
			void setAddr(struct sockaddr_in &addr) { _addr = addr; }
			bool isTLS() const { return _isTLS; } 
			void setTLS(bool isTLS) { _isTLS = isTLS; }

			int recv(void* buf, size_t len, unsigned int timeout);
			int send(const void* buf, size_t len);

		private:
			bool _isTLS;
			int _socket;
			struct sockaddr_in _addr;
	};

}
