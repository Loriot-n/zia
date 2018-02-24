#include "ModuleLog.hpp"

namespace zia {

	std::string const ModuleLog::_logName = "zia.log";
	ModuleLog *ModuleLog::_logModule = nullptr;
	std::ofstream ModuleLog::_logFile;

	ModuleLog::ModuleLog() {

	}

	bool ModuleLog::config(zia::api::Conf const &) {
		return true;
	}

	bool ModuleLog::exec(zia::api::HttpDuplex &) {
		return true;
	}

	ModuleLog *ModuleLog::getLog() {
		if (_logModule == nullptr) {
			_logModule = new ModuleLog();
			_logFile.open(_logName.c_str(), std::ofstream::out | std::ofstream::app);
		}
		return _logModule;
	}

	void ModuleLog::log(zia::logLevel l, std::string const &msg) {
		std::string level;
		switch (l) {
		case DEBUG:
			level = "Debug";
			break;
		case INFO:
			level = "Info";
			break;
		case WARNING:
			level = "Warning";
			break;
		case ERROR:
			level = "Error";
			break;
		}

		_logFile << "[" << level << "] " << msg << std::endl;
	}
};