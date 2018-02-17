#pragma once

#ifdef _WIN32
# include <winsock.h> // Include wsock32.dll for MSVC
#else
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <unistd.h>
#endif

#include "api/Net.hpp"
#include <string.h>
#include <iostream>
#include <vector>

#include "SocketException.hpp"

namespace zia {

	struct ImplSocket {

    };

	class Socket {

		public:
			Socket();
			Socket(uint64_t sizeMax);
			virtual ~Socket();

			/**
			* Call socket, init _sock and setsockopt()
			*/
			bool create(uint64_t sizeMax);

			/** 
			* fill _addr struct and bind to port
			* @param the port number
			*/
			bool bind(const int port);

			/** 
			* Just listen on the socket
			*/
			bool listen() const;

			/* 
			* Accept new connection 
			*/
			bool accept(Socket &) const;

			/* 
			* Check validity of socket
			*/
			bool isValid() const;

			/*
			* Write msg on socket
			* @param Raw type (byte vector)
			*/
			bool send(const api::Net::Raw &) const;

			/*
			* Read from socket
			* @param Reference to the buffer that will be filled
			*/
			int recv(api::Net::Raw &) const;

		protected:
			int winStartup();

			int _sock;
			sockaddr_in _addr;
			uint64_t _sizeMax;

	};
}
