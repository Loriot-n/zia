#include "Server.hpp"
#include "http/HttpParser.hpp"

namespace zia {


	Server::Server(api::Conf &c) {

		// TODO: Get conf value
		(void)c;
		_port = 4242;
		_timeout = 10;
		_threadPoolSize = std::thread::hardware_concurrency();
		_maxEv = 256;
	}

	Server::~Server() {

	}

	void Server::handleRequest(Raw r, api::NetInfo netInfo) {

		std::time_t tt = std::chrono::system_clock::to_time_t(netInfo.time);
		std::cout << ctime(&tt) <<  "Request from " << netInfo.ip.str << ":" << netInfo.port << std::endl;

		HttpParser p(r, netInfo);

		api::HttpDuplex duplex;

		duplex.info = netInfo;
		duplex.raw_req = r;
		p.parse(duplex.req);
	}

	bool Server::run(Callback cb) {

		std::cout << "Starting up server on port " << _port << std::endl;

		try {

			// Create the sockets
			ReactorPtr listenerReactor(new Reactor(_maxEv));
			ReactorPtr connectionReactor(new Reactor(_maxEv));

			ListenerPtr listener = Listener::create(listenerReactor,
													connectionReactor,
													_timeout,
													_port);
			listener->open();

			// Listening thread : accept socket and create session
			std::thread listenerThread([&]() {
				listenerReactor->handleEvents(cb);
			});

			// Session threads : handle request
			std::vector<std::thread> workers;
			for (unsigned int i = 0 ; i < _threadPoolSize - 1 ; i++) {
				workers.push_back(std::thread([&]() {
					connectionReactor->handleEvents(cb);
				}));
			}

			listenerThread.join();

			std::for_each(workers.begin(), workers.end(),
				[](std::thread &t) {
				    t.join();
				}
			);

		} catch (const ServerException& e) {
			std::cout << "[Fatal] Error : " << e.what() << "\nExiting." << std::endl;
			return false;
		}
		return true;
	}

	bool Server::send(api::ImplSocket *, const api::Net::Raw &) {
		return false;
	}

	bool Server::stop() {
		return true;
	}
}