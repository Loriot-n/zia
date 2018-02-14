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
    DefaultModule* exec()
    {
        std::cout << "hello" << std::endl;
        return new DefaultModule();
    }
}