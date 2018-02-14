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
    void* load()
    {
        std::cout << "hello" << std::endl;
        return nullptr; //(void*) new DefaultModule();
    }
}