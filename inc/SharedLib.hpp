#pragma once

#include <iostream>
#include <dlfcn.h>
#include "IModule.hpp"


typedef IModule* (*Loader)();
namespace zia
{
    class SharedLib
    {
        private:
            void *sharedLib;
        public:
            SharedLib(const std::string &);
            ~SharedLib();
            IModule *load();
    };
}