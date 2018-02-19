#include "socket/Session.hpp"

namespace zia {

	Session::Session() {

	}

	Session::~Session() {

	}

	int Session::recv(api::Net::Raw &req) {

		long pageSize = ::sysconf(_SC_PAGESIZE);
		unsigned char buf[pageSize];
		::bzero(buf, pageSize);
		if (req.empty())
			req.reserve(pageSize);

		int r = 0;
		uint64_t totalRead = 0; 
		while ((r = _stream.recv(buf, pageSize, _timeout)) != 0) {

			if (r == -1)
				throw TransferException(strerror(errno));

			// If we read more than the end of the container, re-allocate enough size
			if (req.capacity() < totalRead && r > 0) {

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

	SockState Session::handleInput(api::Net::Callback cb) {

		api::Net::Raw r;
		api::NetInfo  netinfo;
		cb(r, netinfo);
		return SockState::CLOSE;
	}
}