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
            static LibManager &getInstance();
	    std::pair<std::string const, DynLib> &at(std::string const &key);
            void loadModulesList(std::string const &modulesDir);

        private:
            LibManager() = default;
            void readDir(const std::string &dirName);

            std::string modulesDir;
            std::map<std::string, std::pair<std::string const, DynLib>> modulesList;
	    std::mutex mutex;
    };
}
