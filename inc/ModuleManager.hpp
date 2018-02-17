#pragma once

#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <list>
#include <memory>
#include "api/Http.hpp"
#include "IModule.hpp"
#include "DynLib.hpp"

using  zia::api::HttpDuplex;

typedef struct dirent* t_dir;

namespace zia
{
    class ModuleManager
    {
        public:
            ModuleManager(const std::string &);
            ~ModuleManager() = default;
            void load(const std::string&);
            void process(HttpDuplex&);
            void getModulesList();

        private:
	    std::vector<DynLib> libs;
            std::list<std::unique_ptr<IModule>> modules;
            std::map<std::string, std::string> modulesList;
            std::string modulesDir;

            void readDir(const std::string &dirName);
    };
}
