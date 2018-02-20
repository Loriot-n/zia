#pragma once

#include <iostream>
#include "api/Http.hpp"

using  zia::api::HttpDuplex;

namespace zia
{
    class ModuleManager
    {
        public:
            ModuleManager();
            ~ModuleManager();
            void load(const std::string&);
            void process(const HttpDuplex&);
    };
}
