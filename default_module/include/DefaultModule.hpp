#pragma once
#include <iostream>
#include "IModule.hpp"

class DefaultModule : public IModule
{
    public:
        DefaultModule();
        void exec(const zia::api::HttpDuplex&);
};