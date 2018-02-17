#pragma once

#include "http.hpp"

class IModule
{
    public:
        IModule(int priority): priority(priority) {};
        virtual bool exec(zia::api::HttpDuplex&);
        int getPriority() const;
    private:
        unsigned int priority;
};
