#pragma once

#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <list>
#include <memory>
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
            LibManager(const std::string &modulesDir);
            LibManager &getInstance(const Config &serverConfig);
            std::map<std::string, std::pair<std::string, DynLib>> modulesList;
        private:
            void getModulesList();
            std::string modulesDir;
            void readDir(const std::string &dirName);

    };
}
