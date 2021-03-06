#include "HttpParser.hpp"

namespace zia {

	HttpParser::HttpParser(api::Net::Raw &r) :
	_raw(r)
	{
		_rawStr = _raw.data();
		_endMsg = &*endOfHeader(_raw);
	}

	api::Net::Raw::iterator HttpParser::endOfHeader(zia::api::Net::Raw &r) const {

		std::byte seq[4] = { (std::byte)13, (std::byte)10, (std::byte)13, (std::byte)10 };
		zia::api::Net::Raw eoh(seq, seq + sizeof(seq) / sizeof(std::byte));

		return std::search(r.begin(), r.end(), eoh.begin(), eoh.end());
	}

	bool HttpParser::parseHeader(api::HttpRequest &req) {

		const char *head = reinterpret_cast<char *>(_rawStr);
		const char *tail = reinterpret_cast<char *>(_rawStr);

		// METHOD
		while (tail != _endMsg && *tail != ' ') ++tail;
		std::string method(head, tail);
		req.method = getMethod(method);
		if (req.method == api::http::Method::unknown)
		  throw std::runtime_error("Unknown http method");

		// URI / PATH
		while (tail != _endMsg && *tail == ' ') ++tail;
		head = tail;
		while (tail != _endMsg && *tail != ' ') ++tail;
		req.uri = std::string(head, tail);

		// VERSION
		while (tail != _endMsg && *tail == ' ') ++tail;
		head = tail;
		while (tail != _endMsg && *tail != '\r') ++tail;
		std::string version(head, tail);
		req.version = getVersion(version);
		if (tail != _endMsg) ++tail; // Skip \r
		if (tail != _endMsg) ++tail; // Skip \n

		// HEADERS
		head = tail;
		while (head != _endMsg && *head != '\r') {
			while (tail != _endMsg && *tail != '\r') ++tail;
			const char *sep = (char *)memchr(head, ':', tail - head);
			if (!sep) {
				throw ProtocolException("Malformed header");
				return false;
			}
			const char *val = sep + 1;
			while (val != tail && *val == ' ') ++val;
			std::string headerKey(head, sep);
			std::transform(headerKey.begin(), headerKey.end(), headerKey.begin(), ::toupper);
			req.headers[headerKey] = std::string(val, tail);
			head = tail + 2;
			if (tail != _endMsg) ++tail; // Skip \r
			if (tail != _endMsg) ++tail; // Skip \n
		}

		return true;
	}

	api::http::Method HttpParser::getMethod(const std::string &method) const {

		api::http::Method res;

		if (method == "GET")
			res = api::http::Method::get;
		else if (method == "POST")
			res = api::http::Method::post;
		else if (method == "OPTIONS")
			res = api::http::Method::options;
		else if (method == "HEAD")
			res = api::http::Method::head;
		else if (method == "PUT")
			res = api::http::Method::put;
		else if (method == "DELETE")
			res = api::http::Method::delete_;
		else if (method == "TRACE")
			res = api::http::Method::trace;
		else if (method == "CONNECT")
			res = api::http::Method::connect;
		else
			res = api::http::Method::unknown;
		return res;
	}

	api::http::Version HttpParser::getVersion(const std::string &version) const {

		api::http::Version res;
		if (version == "HTTP/0.9")
			res = api::http::Version::http_0_9;
		else if (version == "HTTP/1.0")
			res = api::http::Version::http_1_0;
		else if (version == "HTTP/1.1")
			res = api::http::Version::http_1_1;
		else if (version == "HTTP/2.0")
			res = api::http::Version::http_2_0;
		else
			res = api::http::Version::unknown;

		return res;
	}


	bool HttpParser::parse(api::HttpRequest &req) {

		try {
			if (!parseHeader(req))
				return false;
				// TODO: send 400 Response
		} catch (ServerException &e) {
			std::cerr << e.what() << std::endl;
			return false;
		}

		return true;

	}


}
