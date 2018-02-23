#pragma once

#include "api/Module.hpp"
#include "api/Conf.hpp"
#include "api/Http.hpp"

#include <iostream>
#include <fstream>
#include <cstdarg>

#define LOG zia::ModuleLog::getLog()

namespace zia {

	enum logLevel {
		DEBUG,
		INFO,
		WARNING,
		ERROR
	};

	class ModuleLog : public zia::api::Module {
	private:
		ModuleLog();

		static std::string const _logName;
		static ModuleLog *_logModule;
		static std::ofstream _logFile;

	public:
		bool config(zia::api::Conf const &);
		bool exec(zia::api::HttpDuplex &);

		static ModuleLog *getLog();
		void log(zia::logLevel, std::string const &);
	};
};