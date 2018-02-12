#pragma once

#include <iostream>
#include "api/Net.hpp"
#include "api/Conf.hpp"
#include "Config.hpp"

namespace zia 
{
    class WorkerManager : public api::Net
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
