#include <iostream>

#include "DefaultModule.hpp"

DefaultModule::DefaultModule() : IModule(5)
{
}

bool DefaultModule::config(const zia::api::Conf &conf)
{
    (void) conf;
    return false;
}

bool DefaultModule::exec(zia::api::HttpDuplex& duplex)
{
    std::cout << "hello" << std::endl;
    //(void) duplex;
    return false;
}

extern "C"
{
    IModule* create()
    {
        return new DefaultModule();
    }
}