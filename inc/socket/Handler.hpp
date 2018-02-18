#pragma once

#include <memory> // Shared ptr

#include "api/Net.hpp"

namespace zia {

	class Reactor;
	class Handler : public std::enable_shared_from_this<Handler> {

		public:
			Handler();
			virtual ~Handler();

			Reactor 	*reactor() const { return _reactor; }
			void 	reactor(Reactor *reactor) { _reactor = reactor; }

			virtual int getHandler() const = 0;
			virtual void handleReq(api::Net::Raw, api::NetInfo) = 0;

		private:
			Reactor 	*_reactor;
	};

	typedef std::shared_ptr<Handler> HandlerPtr;
}