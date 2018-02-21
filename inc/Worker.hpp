#pragma once

#include <iostream>
#include "ModuleManager.hpp"

namespace zia
{
    class Worker
    {
        public:
            Worker();
            ~Worker();
            void init();
            void process();
            void send();
            void close();

        private:
            ModuleManager moduleManager;
    };
}
