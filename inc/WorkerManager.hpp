#pragma once

#include <iostream>
#include "Config.hpp"

class WorkerManager 
{
    public:
        WorkerManager(Config config);
        void process();
};