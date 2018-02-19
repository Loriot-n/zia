#include "socket/Listener.hpp"
#include "socket/Stream.hpp"
#include "socket/Session.hpp"

namespace zia {

	Listener::Listener(ReactorPtr server, ReactorPtr session, unsigned int timeout, unsigned int port) :
		_server(server),
		_session(session),
		_timeout(timeout),
		_port(port)
	{

	}

	Listener::~Listener() {
	}


	void Listener::open() {
		if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			throw SocketException("Can't create socket");

		int yes = 1;
		if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
			throw SocketException("Can't set socket options");

		sockaddr_in serveraddr;
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = INADDR_ANY;
		serveraddr.sin_port = htons(_port);

		memset(&(serveraddr.sin_zero), '\0', 8);

		if (bind(_socket, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
			throw SocketException(std::string("Bind :") + std::string(strerror(errno)));
		if (listen(_socket, SOMAXCONN) == -1)
			throw SocketException("Can't listen on the socket");
		struct timeval tv;

		tv.tv_sec = _timeout;
		tv.tv_usec = 0;

		setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));

		//  acceptor_reactor_->register_handler(this->get_ptr());
		_server->registerSock(shared_from_this());
	}

	void Listener::accept(Stream &s) {
		int socket;
		struct sockaddr_in clientaddr;
		unsigned int addrlen = sizeof(clientaddr);

		if ((socket = ::accept(_socket, (struct sockaddr *)&clientaddr, &addrlen)) == -1)
			throw SocketException("Can't accept connection");

		s.setSock(socket);
	}

	void Listener::acceptNewClient() {


	}

	SockState Listener::handleInput(api::Net::Callback cb) {
		(void)cb;
		SessionPtr session(new Session());
		accept(session->stream());
		_session->registerSock(session);
		return SockState::RESUME;
	}


}