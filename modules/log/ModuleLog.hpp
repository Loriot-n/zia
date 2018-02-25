#pragma once

#include "api/Module.hpp"
#include "api/Conf.hpp"
#include "api/Http.hpp"

#include <iostream>
#include <fstream>
#include <cstdarg>

namespace zia {
	
	class ModuleLog : public IModule {
	private:
		ModuleLog() : IModule(0) {};

		static std::string const _logName;
		static std::ofstream _logFile;

	public:
		bool config(zia::api::Conf const &) override;
		bool exec(zia::api::HttpDuplex &) override;
	};
};