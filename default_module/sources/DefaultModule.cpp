#include <iostream>

#include "DefaultModule.hpp"

DefaultModule::DefaultModule() : IModule(5)
{

}

void DefaultModule::exec(const zia::api::HttpDuplex&)
{
    
}

extern "C"
{
    IModule* create()
    {
        return new DefaultModule();
    }
}