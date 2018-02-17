#pragma once

#include <iostream>
#include "api/Net.hpp"
#include "api/Conf.hpp"
#include "Config.hpp"
#include "Socket.hpp"

namespace zia 
{

    class WorkerManager : public api::Net
    {
        private:
            Config config;
        public:
            WorkerManager();
            void reload();
            void set(std::string const &, api::ConfV);
            bool run(Callback cb);
            bool send(ImplSocket* sock, const Raw& resp);
            bool stop();
    };
}
