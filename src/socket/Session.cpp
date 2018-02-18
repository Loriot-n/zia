#include "socket/Session.hpp"

namespace zia {

	Session::Session() {

	}

	Session::~Session() {

	}

	int Session::recv(api::Net::Raw &req) const {

		long pageSize = ::sysconf(_SC_PAGESIZE);
		unsigned char buf[pageSize];
		::bzero(buf, pageSize);
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

	void Session::handleReq(api::Net::Raw r, api::NetInfo netinf) {

		std::cout << "toto" << std::endl;
	}
}