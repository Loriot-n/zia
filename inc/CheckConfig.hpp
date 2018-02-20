#pragma once

#include <iostream>
#include "Conf.hpp"
#include "Config.hpp"

namespace zia
{
    class CheckConfig
    {
        CheckConfig(zia::Config &conf) : conf(conf){};

        template<class Type>
        void check(std::string &name, Type defaultValue)
        {
            if (conf.find(name) == conf.end())
            {
                this->conf.set<Type>(name, defaultValue);
            }
        };

        private:
            zia::Config conf;
    };
}
