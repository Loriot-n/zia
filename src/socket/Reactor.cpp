#include "socket/Reactor.hpp"
#include "socket/Listener.hpp"

#include "assert.h"

namespace zia {

	Reactor::Reactor(int maxEv) {
		
		_epfd = ::epoll_create(maxEv);
		if (_epfd == -1) {
	   		throw SocketException("Can't create epoll");
	  	}
	}

	Reactor::~Reactor() {
		::close(_epfd);
	}

	void Reactor::registerSock(HandlerPtr handler) {
		std::lock_guard<std::mutex> guard(_mutex);

		struct epoll_event ev;

		ev.events = EPOLLIN | EPOLLONESHOT;
		ev.data.fd = handler->getHandler();

		if (epoll_ctl(_epfd, EPOLL_CTL_ADD, handler->getHandler(), &ev) < 0)
			throw SocketException("Can't register handler");

		handler->reactor(this);
  		_handlers.insert(std::make_pair(handler->getHandler(), handler));
	}

	void Reactor::resumeSock(HandlerPtr handler) {
		std::lock_guard<std::mutex> guard(_mutex);

		struct epoll_event ev;

	  	ev.events = EPOLLIN | EPOLLONESHOT;
	  	ev.data.fd = handler->getHandler();

	  	if (epoll_ctl(_epfd, EPOLL_CTL_MOD, handler->getHandler(), &ev) < 0) 
	    	throw SocketException("Can't resume handler");
	}

	void Reactor::deleteSock(HandlerPtr handler) {
		std::lock_guard<std::mutex> guard(_mutex);

		struct epoll_event ev;

		ev.events = 0;
		ev.data.fd = handler->getHandler();

		if (epoll_ctl(_epfd, EPOLL_CTL_DEL, handler->getHandler(), &ev) < 0) 
			throw SocketException("Can't remove handler");

		auto pos = _handlers.find(handler->getHandler());
		if (pos == _handlers.end())
			throw SocketException("Handler not in list");

		handler->reactor (0);
		_handlers.erase(pos);
	}

	void Reactor::handleEvents(api::Net::Callback cb) {

		::bzero(&_epev, sizeof(_epev));
		while (1) {
			
			int ndfs = epoll_wait(_epfd, &_epev, 1, -1);
			if (ndfs == -1) {
				if (errno == EINTR) continue;
				throw SocketException(strerror(errno));
			}

			for (int i = 0; i < ndfs; i++) {
				
				auto pos = _handlers.find(_epev.data.fd);
				if (pos == _handlers.end())
					throw SocketException("Reactor failed to find handler");

				if (_epev.events & EPOLLHUP) {
					struct epoll_event ev;

					if (epoll_ctl(_epfd, EPOLL_CTL_DEL, _epev.data.fd, &ev) < 0)
						throw SocketException("Reactor failed to delete handler");

				} else if (_epev.events & EPOLLIN) {
					try {
						api::Net::Raw r;
						api::NetInfo info;
						auto listener = std::dynamic_pointer_cast<Listener>(pos->second);
						if (listener && listener->getHandler() == pos->first) {
							std::cout << "Listener" << std::endl;
							listener->acceptNewClient();
							resumeSock(pos->second);
						} else {
							api::Net::Raw r;
							api::NetInfo info;
							cb(r, info);
							deleteSock(pos->second);
						}
					} catch(std::exception& e) {
						deleteSock(pos->second);
						throw e;
					}
				}
			}
		}

	}
}