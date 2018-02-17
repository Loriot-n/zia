#include "WorkerManager.hpp"

using zia::api::ConfValue;

namespace zia
{
    WorkerManager::WorkerManager() : config("./config.json")
    {
<<<<<<< HEAD
=======
		//std::to_string();
>>>>>>> 6994785a68fbaf3c16947acafeda9c345624a156
    }

    void WorkerManager::set(std::string const &, api::ConfV v)
    {
        ConfValue confV(v);
    }

    bool WorkerManager::run(Callback cb)
    {
        (void) cb;
        return (true);
    }

    bool WorkerManager::send(ImplSocket* sock, const api::Net::Raw& resp)
    {
        (void) sock;
        (void) resp;
        return (true);
    }

    bool WorkerManager::stop()
    {
        return (true);
    }
}