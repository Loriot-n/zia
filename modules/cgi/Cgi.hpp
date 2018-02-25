#pragma once

#include "IModule.hpp"
#include <sstream>
#include <functional>
#include <unistd.h>
#include <string.h>

namespace zia {

	class Cgi : public IModule {

		public:
			Cgi();

			bool config(api::Conf const &) override;
			bool exec(api::HttpDuplex &) override;

		private:
			bool _childProcess(api::HttpDuplex &);
			bool _parentProcess(api::HttpDuplex &);
			std::string _replaceDash(const std::string &, const char *, const char *) const;
			void _fillEnv(api::HttpDuplex &);


			pid_t 	_pid;
			int 	_outfd[2];
			int 	_infd[2];
			int 	_serverPort;

			char * envp[];
	};
}