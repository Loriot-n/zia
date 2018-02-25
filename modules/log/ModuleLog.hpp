#pragma once

#include "IModule.hpp"
#include "api/Module.hpp"
#include "api/Conf.hpp"
#include "api/Http.hpp"

#include <iostream>
#include <fstream>
#include <cstdarg>

namespace zia {
	
	class ModuleLog : public IModule {
	private:

		std::string _logName;
		std::ofstream _logFile;

	public:
		ModuleLog();
		bool config(zia::api::Conf const &) override;
		bool exec(zia::api::HttpDuplex &) override;
	};
};