#pragma once

#include "http.hpp"

class IModule
{
    public:
        virtual bool exec(zia::api::HttpDuplex&) = 0;
        int getPriority() const { return this->priority; };
    protected:
        unsigned int priority;
};
