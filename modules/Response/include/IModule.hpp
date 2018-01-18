#pragma once

#include "http.hpp"

class IModule
{
    public:
        IModule(int priority): priority(priority) {};
        virtual exec(const zia::api::HttpDuplex&);
        int getPriority() const;
    private:
        unsigned int priority;
};