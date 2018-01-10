#pragma once

#include <iostream>
#include "Config.hpp"
#include "api/conf.hpp"

namespace zia 
{
    class WorkerManager 
    {
        private:
            Config config;
        public:
            WorkerManager();
            void init();
            void reload();
            void set(std::string const &, api::ConfV);
            void run();
    };
}
