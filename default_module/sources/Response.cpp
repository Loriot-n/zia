#include <iostream>

extern "C" 
{
    void* load()
    {
        std::cout << "hello" << std::endl;
        return nullptr; //(void*) new DefaultModule();
    }
}