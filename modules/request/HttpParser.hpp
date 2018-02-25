#pragma once

#include "api/Http.hpp"
#include "exception/ProtocolException.hpp"

#include <algorithm>
#include <sstream>
#include <string.h>


namespace zia {

	class HttpParser {

		public:
			HttpParser(api::Net::Raw &);
			~HttpParser() {};

			bool	parse(api::HttpRequest &);

		private:
			api::Net::Raw::iterator endOfHeader(zia::api::Net::Raw &r) const;
			bool parseHeader(api::HttpRequest &);
			api::http::Method getMethod(const std::string &) const;
			api::http::Version getVersion(const std::string &) const;


			api::Net::Raw 		_raw;
			void 		 		*_rawStr;
			void 				*_endMsg;

	};

}