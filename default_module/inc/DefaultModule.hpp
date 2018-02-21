#pragma once
#include <iostream>
#include "IModule.hpp"
#include "api/Conf.hpp"

class DefaultModule : public IModule
{
    public:
        DefaultModule();
        bool exec(zia::api::HttpDuplex&);
        bool config(const zia::api::Conf& conf);
};