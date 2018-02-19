#pragma once

#include <iostream>

#include "api/Net.hpp"
#include "api/Http.hpp"

namespace zia {

	class RequestHandler {

		public:
			RequestHandler();
			~RequestHandler();

		private:
			api::Net::Raw 	_raw;
			api::NetInfo 	_netInfo;
			api::HttpDuplex _duplex;

	};
}