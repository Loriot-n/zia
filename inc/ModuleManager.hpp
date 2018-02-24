#pragma once

#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <list>
#include <memory>
#include "api/Http.hpp"
#include "IModule.hpp"
#include "DynLib.hpp"
#include "LibManager.hpp"
#include "Config.hpp"

using  zia::api::HttpDuplex;


namespace zia
{
    class ModuleManager
    {
        public:
            ModuleManager(LibManager &);
            void load(const std::string&);
            void process(HttpDuplex&);
        private:
            LibManager &libManager;
            std::list<std::unique_ptr<IModule>> modules;
            std::map<std::string, std::pair<std::string, DynLib>> modulesList;
            std::string modulesDir;

            void readDir(const std::string &dirName);
    };
}
