#include "ModuleLog.hpp"
#include <iomanip>

namespace zia {

	ModuleLog::ModuleLog() : IModule(8) {
	}

	bool ModuleLog::config(zia::api::Conf const &) {
		_logName = "zia.log";
		return true;
	}

	bool ModuleLog::exec(zia::api::HttpDuplex &dup) {
		_logFile.open(_logName, std::ofstream::out | std::ofstream::app);
		std::time_t time = std::chrono::system_clock::to_time_t(dup.info.time);
		std::string ip = dup.info.ip.str;
		std::string port = std::to_string(dup.info.port);

		_logFile << "[" << std::put_time(std::localtime(&time), "%d-%m-%Y %T") << "]\t";
		_logFile << ip << ":" << port << " - New client connected";
		if (dup.info.sock->isTLS) {
			_logFile << " via TLS";
		}
		_logFile << "." << std::endl;
		return true;
	}

	extern "C" IModule *create()
	{
		return new ModuleLog();
	}
}