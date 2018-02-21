#pragma once

#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include "api/Http.hpp"
#include "SharedLib.hpp"
#include <list>
#include "IModule.hpp"
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
            std::list<IModule*> modules;
            std::map<std::string, std::string> modulesList;
            std::string modulesDir;
            void readDir(const std::string &dirName);        
    };
}
