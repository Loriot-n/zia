#include "Cgi.hpp"

#include <thread>

namespace zia {

	Cgi::Cgi() : IModule(4) {
	}

	bool Cgi::config(const api::Conf &c) {
		return true;
	}

	bool Cgi::exec(api::HttpDuplex &duplex) {

		pipe(_outfd);
		pipe(_infd);

		_pid = fork();
		if (_pid == 0) {
			_childProcess(duplex);
		} else if (_pid > 0) {
			_parentProcess(duplex);
		} else {
			perror("Fork");
			return false;
		}
		return true;
	}

	bool Cgi::_parentProcess(api::HttpDuplex &duplex) {

		close(_outfd[0]);
		close(_infd[1]);

		write(_outfd[1], duplex.req.body.data(), duplex.req.body.size());

		char buf[512];
		int r = 0;
		while ((r = read(_infd[0], buf, 512)) > 0)
			duplex.resp.body.insert(duplex.resp.body.end(), (std::byte *)&buf[0], (std::byte *)&buf[r - 1]);

		close(_infd[0]);
		close(_outfd[0]);
		return (true);
	}

	bool Cgi::_childProcess(api::HttpDuplex &duplex) {

		close(STDOUT_FILENO);
		close(STDIN_FILENO);

		dup2(_outfd[0], STDIN_FILENO);
		dup2(_infd[1], STDOUT_FILENO);

		// Write stdout

		close(_outfd[0]);
		close(_outfd[1]);
		close(_infd[0]);
		close(_infd[1]);

		_fillEnv(duplex);

		char * argv[] = {};

		if (execve("/usr/bin/php-cgi", argv, envp) < 0)
			perror("Execve");
		return true;
	}

	void Cgi::_fillEnv(api::HttpDuplex &duplex) {

		std::string hostname(512, '\0');

		::gethostname(&hostname[0], 512);

		std::map<std::string, std::string> cgiEnv =  {
			{"CONTENT_LENGTH", ""},
			{"CONTENT_TYPE", ""},
			{"PATH", ""},
			{"GATEWAY_INTERFACE", "CGI/1.1"},
			{"REDIRECT_STATUS", "true"},
			{"PATH_INFO", ""},
			{"PATH_TRANSLATED", ""},
			{"QUERY_STRING", ""},
			{"REMOTE_ADDR", duplex.info.ip.str},
			{"REMOTE_PORT", std::to_string(duplex.info.port)},
			{"REQUEST_METHOD", ""},
			{"SCRIPT_NAME", ""},
			{"SERVER_NAME", hostname},
			{"SERVER_PORT", ""},
			{"SERVER_PROTOCOL=HTTP/1.1", ""},
			{"SERVER_SOFTWARE=ZIA/0.1", ""}
		};

		for (auto it : duplex.req.headers) {

			std::string env("HTTP_");
			std::string key(_replaceDash(it.first, "_", "-"));

			cgiEnv[key] = it.second;
		}

		if (duplex.req.body.size() > 0)
			cgiEnv["CONTENT_LENGTH"] = std::string(std::to_string(duplex.req.body.size()));

		auto searchContentType = duplex.req.headers.find("HTTP_CONTENT_TYPE");
		if (searchContentType != duplex.req.headers.end())
			cgiEnv["CONTENT_TYPE"] = std::string(searchContentType->second);

		auto searchQueryStr = duplex.req.uri.find('?');
		if (searchQueryStr != std::string::npos)
			cgiEnv["QUERY_STRING"] = std::string(searchQueryStr, duplex.req.uri.back());

		if (duplex.req.method == api::http::Method::get)
			cgiEnv["REQUEST_METHOD"] = "GET";
		else if (duplex.req.method == api::http::Method::post)
			cgiEnv["REQUEST_METHOD"] = "POST";

		int i = 0;
		std::for_each(cgiEnv.begin(), cgiEnv.end(), [&](const auto &map) {
			envp[i++] = strdup(std::string(map.first + '=' + map.second).c_str());
		});
		envp[i + 1] = NULL;
	}

	std::string Cgi::_replaceDash(const std::string &src, const char *to, const char *from) const {

		size_t pos = 0;
		std::string res(src);
		while ((pos = src.find(from, pos)) != std::string::npos) {
			res.replace(pos, 1, to);
			pos += 1;
		}
		return res;
	}

}