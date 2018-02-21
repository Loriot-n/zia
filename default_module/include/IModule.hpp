#pragma once

#include "http.hpp"

class IModule
{
    public:
        IModule(int a) : priority(a) {};
        virtual bool exec(zia::api::HttpDuplex&) = 0;
        int getPriority() const { return this->priority; };
    protected:
        unsigned int priority;
};
