#include "WorkerManager.hpp"

using zia::api::ConfValue;

namespace zia
{
    WorkerManager::WorkerManager() : config("./config.json")
    {
		//std::to_string();
    }

    void WorkerManager::set(std::string const &, api::ConfV v)
    {
        ConfValue confV(v);
    }

    void WorkerManager::init()
    {
    }


    void WorkerManager::run()
    {

    }
}
