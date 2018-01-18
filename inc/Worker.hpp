#pragma once

#include <iostream>
#include "ModuleManager.hpp"

namespace zia
{
    class Worker
    {
        private:
            ModuleManager moduleManager;
        public:
            Worker();
            ~Worker();
            void init();
            void process();
            void send();
            void close();
    };
}
