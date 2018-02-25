#include "Server.hpp"
#include "http/HttpParser.hpp"
#include "ModuleManager.hpp"

namespace zia {


	Server::Server(api::Conf &c) {

		// TODO: Get conf value
		(void)c;
		_port = 4242;
		_sslPort = 4343;
		_timeout = 1;
		_threadPoolSize = std::thread::hardware_concurrency();
		_maxEv = 256;
	}

	Server::~Server() {

	}

	void Server::handleRequest(api::Net::Raw r, api::NetInfo netInfo) {

		(void)r;

		api::HttpDuplex duplex;

		duplex.info = netInfo;

		// Module LOG
		std::time_t tt = std::chrono::system_clock::to_time_t(netInfo.time);
		std::cout << "\n" << ctime(&tt) <<  "Request from " << netInfo.ip.str << ":" << netInfo.port << std::endl;

		// Module REQ
		if (netInfo.sock->isTLS)
			std::cout << "TLS Connection established" << std::endl;


		// ModuleManager &moduleManager = ModuleManager::getInstance();
		// moduleManager.load("php-cgi");
		// moduleManager.process(duplex);

	}

	bool Server::run(Callback cb) {

		std::cout << "Starting up server on port " << _port << std::endl;

		try {

			// Create the sockets
			ReactorPtr listenerReactor(new Reactor(_maxEv));
			ReactorPtr sslListenerReactor(new Reactor(_maxEv));
			ReactorPtr connectionReactor(new Reactor(_maxEv));

			ListenerPtr listener = Listener::create(listenerReactor,
													connectionReactor,
													_timeout,
													_port,
													false);
			listener->open();

			// TLS Listener
			ListenerPtr sslListener = Listener::create(listenerReactor,
													connectionReactor,
													_timeout,
													_sslPort,
													true);
			sslListener->open();


			// Listening thread : accept socket and create session
			std::thread sslListenerThread([&]() {
				sslListenerReactor->handleEvents(cb);
			});

			std::thread listenerThread([&]() {
				listenerReactor->handleEvents(cb);
			});

			// Session threads : handle request
			std::vector<std::thread> workers;
			for (unsigned int i = 0 ; i < _threadPoolSize - 2 ; i++) {
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
