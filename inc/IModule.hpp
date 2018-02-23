#pragma once

#include "api/Http.hpp"
#include "api/Conf.hpp"
#include "api/Module.hpp"
class IModule : public zia::api::Module
{
    public:
        IModule(int priority): priority(priority) {};
        int getPriority() const {return this->priority;};
    private:
        unsigned int priority;
};
