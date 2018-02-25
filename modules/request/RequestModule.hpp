#pragma once
#include <iostream>

#include <string.h>

#include "IModule.hpp"
#include "api/Conf.hpp"
#include "api/Http.hpp"
#include "api/Net.hpp"

#include "http/HttpParser.hpp"

#include "exception/ServerException.hpp"

namespace zia {

	class RequestModule : public IModule
	{
	    public:
	        RequestModule();
	        bool exec(api::HttpDuplex&);
	        bool config(const api::Conf& conf);

	    private:
	    	bool _readBody(api::HttpDuplex &, long long);
	    	bool _readHeader(api::HttpDuplex &);
			long long _getRequestSize(api::HttpDuplex &) const;
			api::Net::Raw::iterator _endOfHeader(api::Net::Raw &r) const;

	    	api::ImplSocket *_sock;
	    	long long _timeout;
	    	uint64_t _pageSize;


	};
}