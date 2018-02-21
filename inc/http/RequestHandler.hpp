#pragma once

#include <iostream>

#include "api/Net.hpp"
#include "api/Http.hpp"

namespace zia {

	class RequestHandler {

		public:
			RequestHandler();
			~RequestHandler();

			static void handleRequest(api::Net::Raw r, api::NetInfo);

		private:
			api::Net::Raw 	_raw;
			api::NetInfo 	_netInfo;
			api::HttpDuplex _duplex;

	};
}