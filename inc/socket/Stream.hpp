#pragma once

#include <vector>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace zia {
	class Stream
	{
		public:
			~Stream();
			Stream();

			int getSock() const { return _socket; }
			void setSock(int socket) { _socket = socket; }

			int recv(void* buf, size_t len, unsigned int timeout);
			int send(const void* buf, size_t len);

		private:
			int _socket;
	};

}
