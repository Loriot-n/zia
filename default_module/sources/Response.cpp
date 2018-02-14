#include <iostream>
#include "DefaultModule.hpp"

extern "C" 
{
    DefaultModule* load()
    {
        std::cout << "hello" << std::endl;
        // return nullptr; //(void*) new DefaultModule();
        return (new DefaultModule);

    }
}