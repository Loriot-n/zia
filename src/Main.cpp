#include "Main.hpp"
#include "WorkerManager.hpp"
#include "Config.hpp"


using zia::WorkerManager;

int Main::main(const int ac, const std::string *av)
{
    try
    {
      zia::Config d("oui.json");
      return 0;
        WorkerManager *workerManager =  new WorkerManager();
        //workerManager.reload();
        workerManager->init();
        workerManager->set(std::string("port") , (long long) 5);
        workerManager->run();

        //Signaux KILL,..
        delete workerManager;
    }
    catch (std::runtime_error const &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}
