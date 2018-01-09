#pragma once

#include <iostream>
#include "api/http.hpp"

using  zia::api::HttpDuplex;

class ModuleManager
{
    public:
        ModuleManager();
        ~ModuleManager();
        void load(const std::string&);
        void process(const HttpDuplex&);
};