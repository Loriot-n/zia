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

using  zia::api::HttpDuplex;

typedef struct dirent* t_dir;

namespace zia
{
    class ModuleManager
    {
        public:
            static ModuleManager &getInstance();
            ModuleManager(const std::string &);

            void load(const std::string&);
            void process(HttpDuplex&);
            void getModulesList();

        private:
            std::list<std::unique_ptr<IModule>> modules;
            std::map<std::string, std::pair<std::string, DynLib>> modulesList;
            std::string modulesDir;

            void readDir(const std::string &dirName);
    };
}
