#include "Main.hpp"
#include "WorkerManager.hpp"
#include "ModuleManager.hpp"


using zia::WorkerManager;

int Main::main(const int ac, const std::string *av)
{
    try
    {
        /*
        WorkerManager *workerManager =  new WorkerManager();
        //workerManager.reload();
        workerManager->init();
        workerManager->set(std::string("port") , (long long) 5);
        workerManager->run();
    
        //Signaux KILL,..
        delete workerManager;*/

        zia::ModuleManager *md = new zia::ModuleManager("./responses");
    }
    catch(std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}