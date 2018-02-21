#pragma once

#include "Server.hpp"
#include "api/Http.hpp"
#include "exception/ProtocolException.hpp"

#include <sstream>


namespace zia {

	class HttpParser {

		public:
			HttpParser(api::Net::Raw &, api::NetInfo &);
			~HttpParser() {};

			bool	parse(api::HttpRequest &);

		private:
			bool parseHeader(api::HttpRequest &);
			api::http::Method getMethod(const std::string &) const;
			api::http::Version getVersion(const std::string &) const;


			api::Net::Raw 		_raw;
			api::NetInfo 		_info;
			char 		 		*_rawStr;
			char 				*_endMsg;

	};

}