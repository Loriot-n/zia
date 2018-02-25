#include "RequestModule.hpp"


namespace zia {

	RequestModule::RequestModule() : IModule(6)
	{
	    std::cout << "Initializing RequestModule" << std::endl;
		_pageSize = ::sysconf(_SC_PAGESIZE);
	}

	bool RequestModule::config(const api::Conf &conf)
	{
	    (void) conf;

	    // _timeout = conf.get(timeout);
	    _timeout = 1;
	    return false;
	}

	bool RequestModule::exec(api::HttpDuplex &duplex)
	{
	    std::cout << "Executing RequestModule" << std::endl;

	    _sock = duplex.info.sock;
	    return _readHeader(duplex);
	}

	bool RequestModule::_readHeader(api::HttpDuplex &duplex) {


		char buf[_pageSize];
		::bzero(buf, _pageSize);

		int read = _sock->stream->recv(buf, _pageSize, _timeout);
		duplex.raw_req.insert(duplex.raw_req.end(),
			(std::byte *)&buf[0], (std::byte *)&buf[read - 1]);

		auto headerEnd = _endOfHeader(duplex.raw_req);
		int dis = std::distance(headerEnd, duplex.raw_req.begin());
		if (dis >= (int)_pageSize) {
			duplex.resp.status = api::http::common_status::request_entity_too_large;
			duplex.resp.reason = std::string("Header too large\r\n");
			std::cerr << "Header" << std::endl;
			return false;
		}

		HttpParser *p = new HttpParser(duplex.raw_req);
		p->parse(duplex.req);

		if (headerEnd == duplex.raw_req.end())
			return true;

		long long toRead;
		if ((toRead = _getRequestSize(duplex)) >= 0) {

			// Read moar
			toRead = toRead - _pageSize;
			try {
				_readBody(duplex, toRead);
			} catch (const std::exception &e) {
				std::cerr << "readBody catched" << std::endl;
				return false;
			}
		}

		else {  // No Content-Length

		}

		return true;
	}

	bool RequestModule::_readBody(api::HttpDuplex &duplex, long long toRead) {

		int nbLoop = -1;
		if (toRead > 0)
			nbLoop = toRead / _pageSize;

		char buf[_pageSize];
		::bzero(buf, _pageSize);

		while (1) {

			int read = _sock->stream->recv(buf, _pageSize, _timeout);
			duplex.raw_req.insert(duplex.raw_req.end(),
				(std::byte *)&buf[0], (std::byte *)&buf[read - 1]);

			if (read < 0) {
				throw ServerException("Error while reading\n");
				return false;
			}
			if (nbLoop == -1)
				continue ;

			nbLoop--;
			if (nbLoop == 0)
				break ;
		}
		return true;
	}

	api::Net::Raw::iterator RequestModule::_endOfHeader(api::Net::Raw &r) const {

		std::byte seq[4] = { (std::byte)13, (std::byte)10, (std::byte)13, (std::byte)10 };
		api::Net::Raw eoh(seq, seq + sizeof(seq) / sizeof(std::byte));

		return std::search(r.begin(), r.end(), eoh.begin(), eoh.end());
	}

	long long RequestModule::_getRequestSize(api::HttpDuplex &duplex) const {

		auto it = duplex.req.headers.find("CONTENT-LENGTH");
		if (it != duplex.req.headers.end())
			return std::atoll(it->second.c_str());
		else
			return 0;
	}

	extern "C"
	{
	    IModule* create()
	    {
	        return new RequestModule();
	    }
	}

}