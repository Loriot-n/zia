#pragma once

#include "http.hpp"

class IModule
{
    public:
        IModule(int priority): priority(priority) {};
        virtual void exec(const zia::api::HttpDuplex&);
        int getPriority() const {return this->priority;};
    private:
        unsigned int priority;
};
