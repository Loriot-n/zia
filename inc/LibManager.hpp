#pragma once

#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include "api/Http.hpp"
#include "IModule.hpp"
#include "DynLib.hpp"
#include "Config.hpp"

typedef struct dirent* t_dir;

namespace zia
{
    class LibManager
    {
        public:
            static LibManager &getInstance(const Config &serverConfig);
	    std::pair<std::string const, DynLib> &at(std::string const &key);

        private:
            LibManager(const std::string &modulesDir);
            void getModulesList();
            void readDir(const std::string &dirName);

            std::string modulesDir;
            std::map<std::string, std::pair<std::string const, DynLib>> modulesList;
	    std::mutex mutex;
    };
}
