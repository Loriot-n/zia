#pragma once

#include "api/Net.hpp"

namespace zia
{
	class ANet : public api::Net
	{
		public:
			ANet() = default;

			bool run(Callback cb);
	};
}