#pragma once

#include "http.hpp"

class IModule
{
    public:
        IModule(int priority): priority(priority) {};
<<<<<<< HEAD:modules/Response/include/IModule.hpp
        virtual bool exec(zia::api::HttpDuplex&);
        int getPriority() const;
=======
        virtual void exec(const zia::api::HttpDuplex&) = 0;
        int getPriority() const {return this->priority;};
>>>>>>> 6994785a68fbaf3c16947acafeda9c345624a156:default_module/include/IModule.hpp
    private:
        unsigned int priority;
};
